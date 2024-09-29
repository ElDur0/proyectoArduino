from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/datos', methods=['POST'])
def recibir_datos():
    datos = request.get_json()  # Recibir datos JSON
    print(f"Datos recibidos: {datos}")
    return jsonify({"status": "Datos recibidos con éxito"}), 200  # Responder y cerrar conexión

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)