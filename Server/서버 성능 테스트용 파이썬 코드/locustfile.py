

from locust import User, task, between, events
import socket
import time
import Protocol_pb2
import pickle
import struct

def create_packet(packet_id, proto_message):
    # 프로토콜 메시지를 직렬화
    serialized_message = proto_message.SerializeToString()
    message_length = len(serialized_message)
    
    # 패킷 헤더 생성 ('H'는 unsigned short, 즉 uint16을 의미)
    packet_header = struct.pack('HH', message_length, packet_id)
    
    # 패킷 헤더와 직렬화된 메시지를 합침
    packet = packet_header + serialized_message
    return packet

def parse_packet(response_data):
    # 첫 4바이트를 정수로 추출 (네트워크 바이트 오더, Big-endian)
    header = struct.unpack('>I', response_data[:4])[0]
    
    # 나머지 데이터를 Protocol Buffer로 파싱
    s_login_response = Protocol_pb2.S_LOGIN()
    s_login_response.ParseFromString(response_data[4:])
    
    return header, s_login_response

class TcpSocketUser(User):
    wait_time = between(1, 2)

    def on_start(self):
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client.settimeout(5) # 5초 타임아웃 설정
        self.client.connect(('127.0.0.1', 7777))
        print("Connected to server")

    @task
    def send_and_receive(self):
        # Protocol만 직렬화 해서 + 패킷헤더
        login_request = Protocol_pb2.C_LOGIN()
        login_request.playerName = "TestPlayer"
        sendPkt = create_packet(1000, login_request)
        # 총 사이즈는 헤더패킷 포함 14가 나와야 함.

        start_time = time.time()
        try:
            self.client.sendall(sendPkt)
            print("C_LOGIN message sent")
            response_data = self.client.recv(4096)

            # Response time in milliseconds
            total_time = int((time.time() - start_time) * 1000)

            header, s_login_response = parse_packet(response_data)
            

            if s_login_response.success:
                events.request.fire(request_type="TCP", name="C_LOGIN", response_time=total_time, response_length=len(response_data), exception=None, context={})
                print(f"Login Successful, Session ID: {s_login_response.yourSessionID}, Player Name: {s_login_response.playerName}")
            else:
                events.request.fire(request_type="TCP", name="C_LOGIN", response_time=total_time, response_length=len(response_data), exception="Login failed", context={})
                print("Login Failed")

        except socket.timeout:
            total_time = int((time.time() - start_time) * 1000)  # Response time in milliseconds
            events.request.fire(request_type="TCP", name="C_LOGIN", response_time=total_time, response_length=0, exception="Timed out waiting for a response", context={})
            print("Timed out waiting for a response")
        except Exception as e:
            total_time = int((time.time() - start_time) * 1000)  # Response time in milliseconds
            events.request.fire(request_type="TCP", name="C_LOGIN", response_time=total_time, response_length=0, exception=str(e), context={})
            print("Failed to send or receive:", str(e))

