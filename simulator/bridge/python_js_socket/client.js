function showMessage(message) {
  window.setTimeout(() => window.alert(message), 50);
}

function receiveMoves(websocket) {
  websocket.addEventListener("message", ({ data }) => {
    const event = JSON.parse(data);
    showMessage(event.message);
  });
}

window.addEventListener("DOMContentLoaded", () => {
  const websocket = new WebSocket("ws://localhost:8001/");
  receiveMoves(websocket);
});
