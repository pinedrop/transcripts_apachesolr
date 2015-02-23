<div class='transcripts-interactive-transcript'>
    <div class='transcript-player'>
        <div class='video-wrapper'>
	    <div class="embed-responsive embed-responsive-16by9">
            	<?php print $video_tag; ?>
	    </div>
            <?php print render($transcript_controls); ?>
        </div>
        <div class='transcript-wrapper'>
            <?php print render($transcript); ?>
        </div>
    </div>
</div>
