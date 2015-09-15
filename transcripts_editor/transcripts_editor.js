(function($) {
		Drupal.behaviors.transcriptsEditor = {
            attach: function (context, settings) {
                $('[data-transcripts-role=transcript]', context)
                    .addBack('[data-transcripts-role=transcript]')
                    .once('editable-transcript')
                    .each(function () {
                        var trid = $(this).attr('data-transcripts-id');
                        $.fn.editable.defaults.mode = 'inline';
                        var exclude = $.map(Drupal.settings.transcripts_editor.exclude,
                            function(val, i) {
                                return '[data-tier=' + val + ']';
                            }
                        );
                        $('.tier', this).not(exclude.join(',')).each(function() {
                            $(this).editable({
                                'type' : 'textarea',
                                'onblur' : 'ignore',
                                'pk' : $(this).closest('[data-tcuid]').attr('data-tcuid'),
                                'name' : $(this).attr('data-tier'),
                                'url' : Drupal.settings.basePath + 'tcu/up/tier',
                                'success' : function(response, newValue) {
                                    if (response.status == 'error') return response.message;
                                }
                            });
                        });
                    });
            }
        }
})(jQuery);