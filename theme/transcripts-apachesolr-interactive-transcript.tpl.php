<div>
	<div class='transcript-player'>
		<div class='video-wrapper' data-spy='affix' data-offset-top='100'>
			<?php print $video_tag; ?>
			<?php print render($video_controls); ?>
			<?php print render($transcript_controls); ?>
		</div>
		<div class='transcript-wrapper'>
			<ul class='nav nav-tabs' role='tablist'>
				<li class='active'><a href='#transcript-<?php print $trid; ?>' role='tab' data-toggle='tab'>Transcript</a></li>
				<li><a href='#hits-<?php print $trid; ?>' role='tab' data-toggle='tab'>Search</a></li>
			</ul>
			<div class='transcript-content tab-content'>
				<div class='tab-pane active' id='transcript-<?php print $trid; ?>'>
					<?php print render($transcript); ?>
				</div>
				<div class='tab-pane' id='hits-<?php print $trid; ?>'>
					<?php print render($hits); ?>
				</div>
			</div>
		</div>
	</div>
</div>
