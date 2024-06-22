sudo docker build -t my_websocket_project_image .

docker run -d -p 9024:9024 --name my_websocket_project_container my_websocket_project_image