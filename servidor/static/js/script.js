function actualizarDatos() {
    fetch('/datos')
        .then(response => response.json())
        .then(data => {
            document.getElementById('temp').textContent = data.sensor_temperatura + " °C";
            document.getElementById('humedad').textContent = data.sensor_humedad + " %";
            document.getElementById('humedadTierra').textContent = data.sensor_humedad_tierra + " %";
        })
        .catch(error => console.error('Error:', error));
}

// Llamar a la función de actualizar datos cada 5 segundos
setInterval(actualizarDatos, 5000);
