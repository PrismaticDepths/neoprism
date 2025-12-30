import pynput, time, struct

buffer = bytearray()

start = time.perf_counter_ns()

def captured_key_press(key:pynput.keyboard.Key|pynput.keyboard.KeyCode):
	if isinstance(key,pynput.keyboard.KeyCode):
		print(key.vk,"down")
	elif isinstance(key,pynput.keyboard.Key):
		print(key.value.vk,"down")
	buffer.extend(struct.pack("<Q",time.perf_counter_ns()-start))
def captured_key_release(key:pynput.keyboard.Key):
	if isinstance(key,pynput.keyboard.KeyCode|pynput.keyboard.KeyCode):
		print(key.vk,"up")
	elif isinstance(key,pynput.keyboard.Key):
		print(key.value.vk,"up")
	buffer.extend(struct.pack("<Q",time.perf_counter_ns()-start))

kb_listener = pynput.keyboard.Listener(on_press=captured_key_press,on_release=captured_key_release)

kb_listener.start()
time.sleep(10)
kb_listener.join()