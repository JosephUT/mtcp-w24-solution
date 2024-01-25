import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";

let camera, scene, renderer, controls, robot, robot_body, clock;
let laser, raycaster, lines;
let odomPosition = new THREE.Vector3(),
  odomYaw = 0.0;
let socket;

function updateRobotPose(x, y, theta) {
  robot.position.set(x, y, robot.position.z);
  robot.rotation.z = Math.PI / 2 - theta;
}

function gaussianRandom(mean = 0, stdev = 1) {
  const u = 1 - Math.random(); // Converting [0,1) to (0,1]
  const v = Math.random();
  const z = Math.sqrt(-2.0 * Math.log(u)) * Math.cos(2.0 * Math.PI * v);
  // Transform to the desired mean and standard deviation:
  return z * stdev + mean;
}

function initWorld() {
  const geometry0 = new THREE.PlaneGeometry(300, 300);
  const material0 = new THREE.MeshLambertMaterial({ color: 0x00274c, side: THREE.DoubleSide });
  const plane = new THREE.Mesh(geometry0, material0);
  scene.add(plane);

  const geometry = new THREE.BoxGeometry(4, 1, 2);
  const material = new THREE.MeshLambertMaterial({ color: 0xffcb05 });
  const cube = new THREE.Mesh(geometry, material);
  scene.add(cube);
  cube.position.set(1, 3, 1);

  const dirLight = new THREE.DirectionalLight(0xffffff, 0.4);
  dirLight.position.set(0, 0, 1).normalize();
  scene.add(dirLight);

  const pointLight = new THREE.PointLight(0xffffff, 4.5, 0, 0);
  pointLight.position.set(0, 0, 90);
  scene.add(pointLight);
}

function initRobot(radius = 0.35, height = 0.2) {
  const boringGrey = new THREE.MeshLambertMaterial({ color: 0xcccccc });

  const geometry0 = new THREE.BoxGeometry(0.01, 0.01, 0.01);
  robot = new THREE.Mesh(geometry0, boringGrey);
  robot.position.z = 0.005;
  scene.add(robot);
  robot.layers.set(2);

  const geometry1 = new THREE.CylinderGeometry(radius, radius, height);
  robot_body = new THREE.Mesh(geometry1, boringGrey);
  robot_body.rotation.x = Math.PI / 2;
  robot_body.position.z = height / 2;
  robot_body.layers.set(2);
  scene.add(robot_body);
  robot.attach(robot_body);

  const geometry2 = new THREE.BoxGeometry(0.01, 0.01, 0.01);
  laser = new THREE.Mesh(geometry2, boringGrey);
  laser.position.x = radius * 0.8;
  laser.position.z = height + 0.015;
  scene.add(laser);
  laser.layers.set(2);
  robot.attach(laser);

  const geometry2a = new THREE.CylinderGeometry(0.04, 0.04, 0.03);
  const material2a = new THREE.MeshLambertMaterial({ color: 0x333333 });
  const laserBody = new THREE.Mesh(geometry2a, material2a);
  laserBody.rotation.x = Math.PI / 2;
  laserBody.position.x = radius * 0.8;
  laserBody.position.z = height + 0.015;
  scene.add(laserBody);
  laserBody.layers.set(2);
  laser.attach(laserBody);

  raycaster = new THREE.Raycaster();
  lines = new THREE.BufferGeometry();
  const material3 = new THREE.LineBasicMaterial({ color: 0x00ff00 });
  const segments = new THREE.LineSegments(lines, material3);
  scene.add(segments);
  segments.layers.set(2);
}

function updateLaser(angular_range = Math.PI, samples = 50, max_range = 3.0) {
  const origin = new THREE.Vector3();
  laser.getWorldPosition(origin);
  raycaster.far = max_range;

  let min_angle = -angular_range / 2;
  let angle = min_angle;
  let angle_inc = angular_range / (samples - 1);
  const points = [];
  const q = new THREE.Quaternion();
  laser.getWorldQuaternion(q);
  const readings = [];

  for (let angle_i = 0; angle_i < samples; angle_i++) {
    const direction = new THREE.Vector3();
    direction.x = Math.cos(angle);
    direction.y = Math.sin(angle);
    points.push(origin);

    const direction2 = direction.clone();
    direction2.applyQuaternion(q);

    raycaster.set(origin, direction2);
    const intersects = raycaster.intersectObjects(scene.children);
    if (intersects.length && intersects[0].distance < max_range) {
      points.push(intersects[0].point);

      readings.push(gaussianRandom(intersects[0].distance));
    } else {
      direction.multiplyScalar(max_range);
      points.push(laser.localToWorld(direction));
      readings.push(Infinity);
    }

    angle += angle_inc;
  }

  lines.setFromPoints(points);

  let msg = { min_angle: min_angle, max_angle: min_angle + angular_range, ranges: readings };
  if (socket && socket.readyState == 1) socket.send(JSON.stringify({ topic: "scan", msg: msg }));
}

function checkForward(globalMotion) {
  var originPoint = robot.position.clone();
  var vertices = robot_body.geometry.attributes.position.array;
  var vertex = new THREE.Vector3();
  for (let i = 0; i < vertices.length; i = i + 3) {
    vertex.x = vertices[i];
    vertex.y = vertices[i + 1];
    vertex.z = vertices[i + 2];
    var globalVertex = vertex.applyMatrix4(robot.matrix);
    raycaster.set(globalVertex, globalMotion);
    var collisionResults = raycaster.intersectObjects(scene.children);
    if (collisionResults.length > 0) {
      return false;
    }
  }
  return true;
}

function updateRobot(dt, vx = 0.4, vtheta = 0.4) {
  // odomYaw += gaussianRandom(vtheta * dt);
  // robot.rotateZ(vtheta * dt);

  const q = new THREE.Quaternion();
  robot.getWorldQuaternion(q);

  let dx = vx * dt;
  let globalMotion = new THREE.Vector3(dx);
  globalMotion.applyQuaternion(q);
  raycaster.far = dx;

  // if (checkForward(globalMotion)) {
  //   robot.translateX(dx);
  // }
  odomPosition.x += gaussianRandom(globalMotion.x);
  odomPosition.y += gaussianRandom(globalMotion.y);
  // updateLaser();
}

function init() {
  socket = new WebSocket("ws://localhost:13348");

  socket.addEventListener("open", function (event) {
    // socket.send('Connection Established');
  });

  socket.addEventListener("message", function (event) {
    const data = JSON.parse(event.data);
    console.log("data received".concat(data));
    if (data.type === "pose2D") {
        updateRobotPose(data.pos.x, data.pos.y, data.pos.theta);
    }
  });

  scene = new THREE.Scene();
  scene.background = new THREE.Color(0xf0f0ff);
  camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
  camera.layers.enable(1);
  camera.layers.enable(2);
  camera.position.z = 5;

  renderer = new THREE.WebGLRenderer();
  renderer.setSize(window.innerWidth, window.innerHeight);
  document.body.appendChild(renderer.domElement);

  controls = new OrbitControls(camera, renderer.domElement);
  controls.enableDamping = true;
  clock = new THREE.Clock();

  initWorld();
  initRobot();
}

function animate() {
  requestAnimationFrame(animate);
  controls.update();

  var delta = clock.getDelta();
  updateRobot(delta);

  renderer.render(scene, camera);
}

init();
animate();
