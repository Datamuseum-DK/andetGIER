BIND = ":1961" # default bind if not specified by argv[1]

import sys,socket,select,threading,time,logging
logging.basicConfig(level=logging.INFO)

OP_CLEAR_READY = 0xa0
OP_CLEAR_LAMP  = 0xb0
OP_SET_LAMP    = 0xb1

log = logging.getLogger(__name__)

args = sys.argv[1:]
if len(args) > 0: (BIND,args) = (args[0],args[1:])

host, port = BIND.split(":")
port = int(port)

listen_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
listen_socket.bind((host, port))
listen_socket.listen(4)

class Conn:
	def __init__(self, socket):
		self.socket = socket
		self.unsent = bytearray()
conns = []

def spool(bs):
	for b in bs:
		print("TODO SPOOL %d" % b)

def broadcast_byte(b):
	for conn in conns:
		conn.unsent.append(b)

while True:
	log.debug("tick")
	rs, ws, xs = select.select(
		[listen_socket]+[c.socket for c in conns],
		[c.socket for c in conns if len(c.unsent)>0],
		[listen_socket]+[c.socket for c in conns],
		0.02)

	if listen_socket in rs:
		rs.remove(listen_socket)
		s, addr = listen_socket.accept()
		log.info("accepted connection from %s" % ":".join(map(str,addr)))
		conns.append(Conn(s))

	assert listen_socket not in ws
	if listen_socket in xs:
		xs.remove(listen_socket)
		raise RuntimeError("listen socket error")

	for s in rs:
		bs = s.recv(1024)
		if len(bs) > 0:
			spool(bs)
		else:
			log.info("closed connection")
			conns = [c for c in conns if c.socket is not s]

	for s in ws:
		for c in conns:
			if c.socket is not s: continue
			(b, c.unsent) = (c.unsent[0:1], c.unsent[1:])
			s.send(b)

	for s in xs:
		raise RuntimeError("socket error")

