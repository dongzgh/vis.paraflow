from moviepy.video.io.VideoFileClip import VideoFileClip
from moviepy.audio.io.AudioFileClip import AudioFileClip

# Extract audio from video.
node_in_video: VideoFileClip = in_video
node_out_extracted_audio: AudioFileClip = node_in_video.audio
node_out_extracted_video = node_in_video.without_audio()
