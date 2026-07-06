from flask import Flask, jsonify
from spotify_client import get_currently_playing

app = Flask(__name__)
@app.route('/')
def home():
    return "Spotify Auth Server is running!"

@app.route('/current-track')
def current_track():
    track_data = get_currently_playing()
    return jsonify(track_data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)

