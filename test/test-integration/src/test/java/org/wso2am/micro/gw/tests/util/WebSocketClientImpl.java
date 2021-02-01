package org.wso2am.micro.gw.tests.util;


import org.java_websocket.client.WebSocketClient;
import org.java_websocket.handshake.ServerHandshake;

import java.net.URI;
import java.nio.ByteBuffer;
import java.util.concurrent.TimeUnit;

public class WebSocketClientImpl extends WebSocketClient {
    public WebSocketClientImpl(URI serverUri) {
        super(serverUri);
    }

    @Override
    public void addHeader(String key, String value) {
        super.addHeader(key, value);
    }

    @Override
    public void connect() {
        super.connect();
    }

    @Override
    public boolean connectBlocking() throws InterruptedException {
        return super.connectBlocking();
    }

    @Override
    public boolean connectBlocking(long timeout, TimeUnit timeUnit) throws InterruptedException {
        return super.connectBlocking(timeout, timeUnit);
    }

    @Override
    public void close() {
        super.close();
    }

    @Override
    public void send(String text) {
        super.send(text);
    }

    @Override
    public void send(byte[] data) {
        super.send(data);
    }

    @Override
    public void onMessage(ByteBuffer bytes) {
        super.onMessage(bytes);
    }

    @Override
    public boolean isOpen() {
        return super.isOpen();
    }

    @Override
    public boolean isClosed() {
        return super.isClosed();
    }

    @Override
    public void close(int code) {
        super.close(code);
    }

    @Override
    public void close(int code, String message) {
        super.close(code, message);
    }

    @Override
    public void send(ByteBuffer bytes) {
        super.send(bytes);
    }

    @Override
    public void onOpen(ServerHandshake serverHandshake) {

    }

    @Override
    public void onMessage(String s) {

    }

    @Override
    public void onClose(int i, String s, boolean b) {

    }

    @Override
    public void onError(Exception e) {

    }
}
