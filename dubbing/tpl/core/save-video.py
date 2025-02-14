from moviepy.video.io.VideoFileClip import VideoFileClip
import os

# Save video to disk.
node_in_video: VideoFileClip = in_video
node_in_path = 'in_path'
if not os.path.exists(os.path.dirname(node_in_path)):
    os.makedirs(os.path.dirname(node_in_path))
node_in_video.write_videofile(node_in_path)
