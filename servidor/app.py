from flask import Flask, render_template, jsonify, request

app = Flask(__name__)

# Variable para almacenar los datos recibidos del sensor
data_recibida = {}

# Ruta principal que renderiza la página web
@app.route('/')
def index():
    return render_template('index.html')

# Ruta para recibir los datos del sensor en formato JSON
@app.route('/datos', methods=['POST'])
def recibir_datos():
    global data_recibida  # Usar la variable global para almacenar los datos
    data = request.get_json()
    print(f"Datos recibidos: {data}")
    data_recibida = data  # Almacena los datos
    return jsonify({"status": "Datos recibidos con éxito"})

# Ruta para enviar los datos recibidos en formato JSON
@app.route('/datos', methods=['GET'])
def enviar_datos():
    return jsonify(data_recibida)  # Devuelve los datos almacenados

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)
