from moviepy.video.io.VideoFileClip import VideoFileClip
from moviepy.audio.io.AudioFileClip import AudioFileClip
import os

# Load video file from disk.
n930_in_path = '/Users/dong/Documents/github/visParaflowAssets/dubbing/res/tutorial.mp4'
n930_out_video = VideoFileClip(n930_in_path)

# Extract audio from video.
n752_in_video: VideoFileClip = n930_out_video
n752_out_extracted_audio: AudioFileClip = n752_in_video.audio
n752_out_extracted_video = n752_in_video.without_audio()

# Save video to disk.
n532_in_video: VideoFileClip = n752_out_extracted_video
n532_in_path = '/Users/dong/Documents/github/visParaflowAssets/dubbing/out/result.mp4'
if not os.path.exists(os.path.dirname(n532_in_path)):
    os.makedirs(os.path.dirname(n532_in_path))
n532_in_video.write_videofile(n532_in_path)
