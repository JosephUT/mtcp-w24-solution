import * as THREE from 'three';

function setUpRenderer() {
    const canvas = document.querySelector('#c');
    return new THREE.WebGLRenderer({antialias: true, canvas});
}

function setUpCamera() {
    const fov = 75;
    const aspect = 2;
    const near = 0.1;
    const far = 5;
    const camera = new THREE.PerspectiveCamera(fov, aspect, near, far); 
    camera.position.z = 2;
    return camera
}

function setUpScene() {
    return new THREE.Scene();
}

function setUpLight(scene) {
    const color = 0xFFFFFF;
    const intensity = 3;
    const light = new THREE.DirectionalLight(color, intensity);
    light.position.set(-1, 2, 4);
    scene.add(light);
    return light;
}

function setUpRobot(scene) {
    // Set up global geometry for mesh.
    const boxWidth = 1;
    const boxHeight = 1;
    const boxDepth = 1;
    const geometry = new THREE.BoxGeometry(boxWidth, boxHeight, boxDepth);

    // Set up global material for mesh.
    const material = new THREE.MeshPhongMaterial({color: 0x44aa88});

    // Initialize global mesh and add it to the scene.
    const robot = new THREE.Mesh(geometry, material);
    scene.add(robot);
    return robot;
}

// Set up globals for simulation.
const renderer = setUpRenderer();
const camera = setUpCamera();
const global_scene = setUpScene();
const light = setUpLight(global_scene);
const robot = setUpRobot(global_scene);
renderer.render(global_scene, camera);

function setUpMessageCallback(websocket) {
  websocket.addEventListener("message", ({ data }) => {
    const pose = JSON.parse(data);
    robot.position.x = pose.x;
    robot.position.y = pose.y;
    robot.rotation.z = pose.theta;
    renderer.render(global_scene, camera);
  });
}

// Set websocket for communication with the bridge.
const websocket = new WebSocket("ws://localhost:8001/");
setUpMessageCallback(websocket);
