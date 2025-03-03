from moviepy.video.io.VideoFileClip import VideoFileClip
from moviepy.audio.io.AudioFileClip import AudioFileClip
import os

# Load video file from disk.
n656_in_path = '/Users/dong/Documents/github/visParaflowAssets/dubbing/res/tutorial.mp4'
n656_out_video = VideoFileClip(n656_in_path)

# Extract audio from video.
n948_in_video: VideoFileClip = n656_out_video
n948_out_extracted_audio: AudioFileClip = n948_in_video.audio
n948_out_extracted_video = n948_in_video.without_audio()

# Save video to disk.
n636_in_audio: AudioFileClip = n948_out_extracted_audio
n636_in_path = '/Users/dong/Documents/github/visParaflowAssets/dubbing/out/speech.wav'
if not os.path.exists(os.path.dirname(n636_in_path)):
    os.makedirs(os.path.dirname(n636_in_path))
n636_in_audio.write_audiofile(n636_in_path)
