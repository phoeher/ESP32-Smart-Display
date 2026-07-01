# import necessary libraries
from config import CLIENT_ID, REDIRECT_URI, SCOPE
from http.server import BaseHTTPRequestHandler, HTTPServer
import urllib.parse
import webbrowser

# global variable to store the authorization code
auth_code = None

# establish the base URL for Spotify authorization
base_url = "https://accounts.spotify.com/authorize"

# function to retrieve the authorization URL with the necessary parameters
def get_auth_url():
    # creates a dictionary of parameters from the config file
    params = {
    "client_id": CLIENT_ID,
    "response_type": "code",
    "redirect_uri": REDIRECT_URI,
    "scope": SCOPE
    }
    
    # creates the full url by combining the base with the encoded parameters
    full_url = base_url + "?" + urllib.parse.urlencode(params)
    return full_url

# function to initiate the Spotify authorization flow
def start_auth_flow():
    # sets up a local HTTP server to listen for the callback from Spotify
    server = HTTPServer(('127.0.0.1', 8080), CallbackHandler)

    # retrieves url and opens it in the default web browser for user login
    url = get_auth_url()
    webbrowser.open(url)
    
    # waits for the callback request from Spotify after user login
    server.handle_request()

    return auth_code

# CallbackHandler class to handle the HTTP GET request from Spotify after user login
class CallbackHandler(BaseHTTPRequestHandler):
    def do_GET(self):

        # parses the incoming request to extract the authorization code from the query parameters
        parsed_path = urllib.parse.urlparse(self.path)
        query_params = urllib.parse.parse_qs(parsed_path.query)
        code = query_params["code"][0]
        
        # sets the global variable auth_code to the retrieved authorization code
        global auth_code
        auth_code = code

        # final window to display after successful login
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        self.wfile.write(b"You can close this window now.")

if __name__ == "__main__":
    start_auth_flow()