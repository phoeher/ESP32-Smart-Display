import requests
import base64
import json

from auth import start_auth_flow
from config import CLIENT_ID, CLIENT_SECRET, REDIRECT_URI

# this function creates the authoriztaion header using the client id and secret
def get_auth_header():
    # combines the client id and secret into a single string
    credentials = CLIENT_ID + ":" + CLIENT_SECRET
    # encodes the credentials to bytes
    credentials_bytes = credentials.encode()
    # encodes the bytes to base64 and decodes it back to a string in base64 format
    credentials_base64 = base64.b64encode(credentials_bytes).decode()
    return {
        "Authorization": "Basic " + credentials_base64
    }

# this function exchanges the authorization code for an access token
def exchange_code_for_token(auth_code):
    # established the token url, headers, and data
    token_url = "https://accounts.spotify.com/api/token"
    headers = get_auth_header()
    data = {
        "grant_type": "authorization_code",
        "code": auth_code,
        "redirect_uri": REDIRECT_URI
    }

    # sends a POST request to the token url with the headers and data to retrieve the access token
    response = requests.post(token_url, headers=headers, data=data)

    if response.status_code != 200:
        print("Token exchange failed.")
        print("Status code:", response.status_code)
        print("Response:", response.text)
        return None

    token_data = response.json()  
    
    with open("token.json", "w") as file:
        json.dump(token_data, file, indent=4)
    
    print("Tokens saved to token.json")

if __name__ == "__main__":
    exchange_code_for_token(start_auth_flow())