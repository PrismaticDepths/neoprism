import pynput, time, struct

buffer = bytearray()
start = 0

class Events: # DO NOT MODIFY EXISTING ENUMS SINCE THAT WOULD BREAK OLD RECORDINGS
	KEY_DOWN = 10
	KEY_UP = 11
	MOUSE_DOWN = 20
	MOUSE_UP = 21
	MOUSE_MOVE_ABSOLUTE = 22
	MOUSE_MOVE_RELATIVE = 23

#USES LITTLE ENDIAN IF YOU CANT ALREADY TELL
#HEADER: EVENT TIMESTAMP, EVENT TYPE
#HEADER: UINT64, UINT8,
EVENT_HEADER_FMT = "<QB"
PAYLOAD_FMTS = {
Events.KEY_DOWN:"<H", #UINT16
Events.KEY_UP:"<H", #UINT16
Events.MOUSE_DOWN:"<BHH", #UINT8,UINT16,UINT16
Events.MOUSE_UP:"<BHH", #UINT8,UINT16,UINT16
Events.MOUSE_MOVE_ABSOLUTE:"<HH", #UINT16,UINT16
Events.MOUSE_MOVE_RELATIVE:"<hh", #INT16,INT16
}

def log_event(timestamp,event,payload):
	buffer.extend(struct.pack(EVENT_HEADER_FMT+PAYLOAD_FMTS[event],timestamp,event,payload))

def captured_key_press(key:pynput.keyboard.Key|pynput.keyboard.KeyCode):
	t=time.perf_counter_ns()
	log_event(t,Events.KEY_DOWN,key.vk if isinstance(key,pynput.keyboard.KeyCode) else key.value.vk)
def captured_key_release(key:pynput.keyboard.Key|pynput.keyboard.KeyCode):
	t=time.perf_counter_ns()
	log_event(t,Events.KEY_UP,key.vk if isinstance(key,pynput.keyboard.KeyCode) else key.value.vk)
def captured_mouse_click(x,y,button:pynput.mouse.Button,pressed):
	t=time.perf_counter_ns()
	log_event(t,Events.MOUSE_DOWN if pressed else Events.MOUSE_UP,button.value,x,y)
def captured_mouse_move(x,y):
	t=time.perf_counter_ns()
	log_event(t,Events.MOUSE_MOVE_ABSOLUTE,x,y)
kb_listener = pynput.keyboard.Listener(on_press=captured_key_press,on_release=captured_key_release)
mouse_listener = pynput.mouse.Listener(on_move=captured_mouse_move,)

start = time.perf_counter_ns()
kb_listener.start()
time.sleep(10)
kb_listener.stop()

#chatgpt written for-loop that just prints the data in the bytearray in a human readable way
for i in range(0, len(buffer), struct.calcsize(EVENT_FMT)):
    chunk = buffer[i:i+struct.calcsize(EVENT_FMT)]
    dt_ns, event_type, key_code = struct.unpack(EVENT_FMT, chunk)
    print(f"{dt_ns} {event_type} {key_code}")