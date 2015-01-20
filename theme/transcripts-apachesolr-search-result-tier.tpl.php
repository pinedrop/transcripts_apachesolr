<div data-tier='<?php print $tier_name; ?>' class='tier'>
    <?php if (isset($snippets)): ?>
        <?php print implode('...', $snippets); ?>
    <?php else: ?>
        <?php print $tier_value; ?>
    <?php endif; ?>
</div>
