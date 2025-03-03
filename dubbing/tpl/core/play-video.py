from moviepy.video.io.VideoFileClip import VideoFileClip
import subprocess
import os

# Play video.
node_in_video: VideoFileClip = in_video
if os.name == "nt":  # Windows
    os.startfile(node_in_video.filename)
else:  # macOS & Linux
    subprocess.run(["open" if os.uname().sysname == "Darwin" else "xdg-open", node_in_video.filename])
