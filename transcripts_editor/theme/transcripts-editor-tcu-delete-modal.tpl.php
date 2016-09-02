<div class="modal fade tcu-delete-modal" id="tcu-delete-modal-<?php print $trid; ?>" tabindex="-1" role="dialog">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal"><?php print $closeicon; ?></button>
                <h4 class="modal-title"><?php print $title; ?></h4>
            </div>
            <div class="modal-body">
                <?php print $message; ?>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default tcu-delete-cancel" data-dismiss="modal"><?php print $no; ?></button>
                <button type="button" class="btn btn-primary tcu-delete-confirm"><?php print $yes; ?></button>
            </div>
        </div> <!-- modal content -->
    </div> <!-- modal dialog -->
</div> <!-- modal body -->