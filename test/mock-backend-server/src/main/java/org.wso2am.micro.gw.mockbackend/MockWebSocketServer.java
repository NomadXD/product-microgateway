package org.wso2am.micro.gw.mockbackend;

import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.java_websocket.WebSocket;
import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;

public class MockWebSocketServer extends Thread {
    private static final Logger logger = Logger.getLogger(MockWebSocketServer.class.getName());
    private final int backendServerPort;
    private final Server server;

    public MockWebSocketServer(int backendServerPort){
        this.backendServerPort = backendServerPort;
        this.server = new Server(new InetSocketAddress(backendServerPort));
    }

    private class Server extends WebSocketServer{
        public Server(InetSocketAddress address) {
            super(address);
        }

        @Override
        public void onMessage(WebSocket conn, ByteBuffer message) {
            conn.send(message);
        }

        @Override
        public void onOpen(WebSocket webSocket, ClientHandshake clientHandshake) {
            webSocket.send("Hello from server");
        }

        @Override
        public void onClose(WebSocket webSocket, int i, String s, boolean b) {
            logger.log(Level.INFO, "websocket connection closed");
        }

        @Override
        public void onMessage(WebSocket webSocket, String s) {
            webSocket.send(s);
        }

        @Override
        public void onError(WebSocket webSocket, Exception e) {
            logger.log(Level.SEVERE, e.toString());
        }

        @Override
        public void onStart() {
            logger.log(Level.INFO, "websocket server started on port:"+getPort());
        }
    }

    @Override
    public void run() {
        if(backendServerPort < 0){
            throw new RuntimeException("Invalid port");
        }
        try{
            server.start();
        }catch (Exception e){
            logger.log(Level.SEVERE, e.toString());
        }
    }
}
