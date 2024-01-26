import * as THREE from 'three';

let lastReceivedMessage = ''; 

function setUpRenderer() {
    const canvas = document.querySelector('#c');
    return new THREE.WebGLRenderer({ antialias: true, canvas });
}

function setUpCamera() {
    const fov = 75;
    const aspect = window.innerWidth / window.innerHeight; 
    const near = 0.1;
    const far = 5;
    const camera = new THREE.PerspectiveCamera(fov, aspect, near, far);
    camera.position.z = 2;
    return camera;
}

function setUpScene() {
    return new THREE.Scene();
}

function setUpLight(scene) {
    const color = 0xFFFFFF;
    const intensity = 3;
    
    const light = new THREE.DirectionalLight(color, intensity);
    light.position.set(0, 0, 1); 
    
    scene.add(light);
    return light;
}

function setUpRobot(scene) {
    const radius = 0.5; 
    const segments = 32; 
    const geometry = new THREE.CircleGeometry(radius, segments);

    const textureLoader = new THREE.TextureLoader();
    const texture = textureLoader.load('logo_img.png');
    const material = new THREE.MeshBasicMaterial({ map: texture });

    const robot = new THREE.Mesh(geometry, material);
    scene.add(robot);
    return robot;
}

const renderer = setUpRenderer();
const camera = setUpCamera();
const global_scene = setUpScene();
const light = setUpLight(global_scene);
const robot = setUpRobot(global_scene);
renderer.render(global_scene, camera);

document.body.style.backgroundColor = 'black';

function setUpMessageCallback(websocket) {
    websocket.addEventListener("message", ({ data }) => {
        const pose = JSON.parse(data);
        robot.position.x = pose.x;
        robot.position.y = pose.y;
        robot.rotation.z = pose.theta;
        console.log(data)
        renderer.render(global_scene, camera);

        lastReceivedMessage = data || lastReceivedMessage;
        displayMessage();
    });
}

let messageDiv;

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

    messageDiv.innerHTML = `<span style="color: lime; font-weight: bold; font-size: 20px;">Received Message: ${lastReceivedMessage}</span>`;
}

const websocket = new WebSocket("ws://localhost:8001/");
setUpMessageCallback(websocket);