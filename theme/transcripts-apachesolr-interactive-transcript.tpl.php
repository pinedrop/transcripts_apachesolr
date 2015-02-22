<div class='transcripts-interactive-transcript'>
    <div class='transcript-player'>
        <div class='video-wrapper'>
            <?php print $video_tag; ?>
            <?php print render($transcript_controls); ?>
        </div>
        <div class='transcript-wrapper'>
            <?php print render($transcript); ?>
        </div>
    </div>
</div>
