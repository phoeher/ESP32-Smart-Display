import requests
import json

from token_exchange import get_auth_header

# this function loads the tokens from the token.json file
def load_tokens():
    with open("token.json", "r") as file:
        return json.load(file)

# this function refreshes the access token using the refresh token
def refresh_access_token():
    tokens = load_tokens()

    # establishes the token url, headers, and data for the refresh request
    token_url = "https://accounts.spotify.com/api/token"
    headers = get_auth_header()
    data = {
        "grant_type": "refresh_token",
        "refresh_token": tokens.get("refresh_token")
    }
    
    # sends a POST request to the token url with the headers and data to retrieve a new access token
    response = requests.post(token_url, headers=headers, data=data)

    if response.status_code != 200:
        print ("Token refresh failed.")
        print("Status code:", response.status_code)
        print("Response:", response.text)
        return None
    
    new_tokens = response.json()
    tokens.update(new_tokens)

    # saves the updated tokens back to the token.json file
    with open("token.json", "w") as file:
        json.dump(tokens, file, indent=4)
    
    print("Tokens updated in token.json")
    return tokens
    
if __name__ == "__main__":
    tokens = load_tokens()
    print(tokens.keys())