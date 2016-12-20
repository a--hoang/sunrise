Web Server
1. Compile using makefile - “make all” will compile the server executable, along with the shared modules
2. To run: ./server -a localhost -p 8080 -v runs the server on localhost:8080
1. If you specify - v (verbose), the server provides information and displays IP address of
each connected client.
2. If you do not specify an address or a port number, the server will use default values.

App
1. Build using Xcode 7. Runs on generic iOS devices. Does not support iPad Pro. Unofficially signed app, so you will need a developer account to build and deploy.
2. To set up app to communicate with server, enter the server name and port number entered when running the server, preceding with http.
3. To access a preset module, touch one of the buttons below. This app prepends Arduino functions only. The app does not support the directory listing function, the histogram function, or the image display functions. These can only be accessed through an html 1.0/1.1 compliant browser.

Current Limitations:
1. Directory listing function does not display output in HTML.
2. App does not error check server URL inputs for accuracy.
