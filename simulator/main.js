import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";

let camera, scene, renderer, controls, robot, robot_body, clock;
let laser, raycaster, lines;
let socket;

function initWorld() {
  const geometry0 = new THREE.PlaneGeometry(300, 300);
  const material0 = new THREE.MeshPhongMaterial({ color: 0x00274c, side: THREE.DoubleSide });
  const plane = new THREE.Mesh(geometry0, material0);
  scene.add(plane);

  const pointLight = new THREE.PointLight(0xffffff, 4.5, 0, 0);
  pointLight.position.set(80, -40, 90);
  scene.add(pointLight);
}

function initRobot(radius = 0.35, height = 0.2) {
  const boringGrey = new THREE.MeshPhongMaterial({ color: 0xcccccc });

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
  const material2a = new THREE.MeshPhongMaterial({ color: 0x333333 });
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

let messageDiv;
let lastReceivedMessage;

function displayMessage() {
    if (!messageDiv) {
        const div = document.createElement('div');
        div.id = 'messageDiv';
        div.style.position = 'absolute';
        div.style.bottom = '10px';
        div.style.width = '100%';
        div.style.textAlign = 'center';
        document.body.appendChild(div);
        messageDiv = div;
    }

    messageDiv.innerHTML = `<span style="color: yellow; font-weight: bold; font-size: 20px;">received pose: ${lastReceivedMessage}</span>`;
}

function setUpMessageCallback(websocket) {
    websocket.addEventListener("message", ({ data }) => {
        const pose = JSON.parse(data);
        console.log(data);

        lastReceivedMessage = data || lastReceivedMessage;
        displayMessage();

        robot.position.x = pose.x;
        robot.position.y = pose.y;
        robot.rotation.z = pose.theta;
        renderer.render(scene, camera);
    });
}

function init() {
  socket = new WebSocket("ws://localhost:8001");
  setUpMessageCallback(socket);

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
  renderer.render(scene, camera);
}

init();
