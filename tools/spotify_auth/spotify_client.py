import requests

from refresh import load_tokens, refresh_access_token

# this function creates the bearer authorization header using the access token
def get_bearer_header():
    tokens = load_tokens()
    access_token = tokens.get("access_token")
    return {
        "Authorization": "Bearer " + access_token
    }

# this function retrieves the currently playing track information from the Spotify API
def get_currently_playing():
    url = "https://api.spotify.com/v1/me/player/currently-playing"
    headers = get_bearer_header()

    # sends a GET request to the Spotify API to retrieve the currently playing track information
    response = requests.get(url, headers=headers)

    # checks if the access token has expired and refreshes it if necessary
    if response.status_code == 401:
        print("Access token expired. Refreshing...")
        refresh_access_token()
        headers = get_bearer_header()
        response = requests.get(url, headers=headers)
        ## eventually handle case where refresh token is also expired or spotify returns an error
    
    # creates a dictionary with the relevant track information from the response JSON
    raw_track_data = response.json()
    track_data = {
       "track_title": raw_track_data["item"]["name"],
       "artists": [artist["name"] for artist in raw_track_data["item"]["artists"]],
       "album": raw_track_data["item"]["album"]["name"],
       "album_art_url": raw_track_data["item"]["album"]["images"][0]["url"],
       "is_playing": raw_track_data["is_playing"],
       "progress_ms": raw_track_data["progress_ms"],
       "duration_ms": raw_track_data["item"]["duration_ms"]
    }

    return track_data

if __name__ == "__main__":
    get_currently_playing()
