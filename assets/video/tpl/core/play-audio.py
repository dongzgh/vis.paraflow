from moviepy.audio.io.AudioFileClip import AudioFileClip
import subprocess
import os

# Play audio.
node_in_audio: AudioFileClip = in_audio
if os.name == "nt":  # Windows
    os.startfile(node_in_audio.filename)
else:  # macOS & Linux
    subprocess.run(["open" if os.uname().sysname == "Darwin" else "xdg-open", node_in_audio.filename])
