import playback
import recorder
import pynput

def toggle_recording_playback():
	pass
def toggle_recording( ):
	pass

with pynput.keyboard.GlobalHotKeys({
		'<ctrl>+<f7>': toggle_recording,
		'<ctrl>+<f9>': toggle_recording_playback}) as h:
	h.join()