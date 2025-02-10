from moviepy.video.io.VideoFileClip import VideoFileClip
import subprocess
import os

# Play video.
node_in_video: VideoFileClip = in_video
video_path = node_in_video.write_videofile(
    "assets/video-processing/tmp/node-in.mp4",
    codec="libx264",
    fps=24,
    threads=4,
    logger=None,
)
if os.name == "nt":  # Windows
    os.startfile(video_path)
else:  # macOS & Linux
    subprocess.run(["open" if os.uname().sysname == "Darwin" else "xdg-open", video_path])
