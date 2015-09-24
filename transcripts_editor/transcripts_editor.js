(function ($) {
    Drupal.behaviors.transcriptsEditor = {
        attach: function (context, settings) {
            $('[data-transcripts-role=transcript]', context)
                .addBack('[data-transcripts-role=transcript]')
                .once('editable-transcript')
                .each(function () {
                    var transcript = this;
                    var trid = $(this).attr('data-transcripts-id');

                    //$.fn.editable.defaults.mode = 'inline';
                    var exclude = $.map(Drupal.settings.transcripts_editor.exclude,
                        function (val, i) {
                            return '[data-tier=' + val + ']';
                        }
                    );
                    $('.tier', this).not(exclude.join(',')).each(function () {
                        $(this).editable({
                            'mode': 'inline',
                            'placement': 'bottom',
                            'showbuttons': 'bottom',
                            'type': 'textarea',
                            'onblur': 'ignore',
                            'pk': $(this).closest('[data-tcuid]').attr('data-tcuid'),
                            'name': $(this).attr('data-tier'),
                            'params': {
                                'oldValue': $(this).text()
                            },
                            'url': Drupal.settings.basePath + 'tcu/up/tier',
                            'success': function (response, newValue) {
                                if (response.status == 'error') return response.message;
                            }
                        });
                    });
                    var displays = Drupal.settings.transcripts_editor.speaker_displays;
                    var speaker_names = {};
                    var transcript_speakers = {};

                    for (var tier_name in displays) {
                        if (displays.hasOwnProperty(tier_name)) {
                            speaker_names[tier_name] = new Bloodhound({
                                datumTokenizer: Bloodhound.tokenizers.whitespace,
                                queryTokenizer: Bloodhound.tokenizers.whitespace,
                                prefetch: Drupal.settings.basePath + 'speakers/' + tier_name
                            });
                            transcript_speakers[tier_name] = [];
                        }
                    }

                    $('.speaker-display', this).each(function () {
                        var tier_name = $(this).attr('data-speaker-display');
                        var speaker_name = $(this).text();
                        if (transcript_speakers[tier_name].indexOf(speaker_name) == -1) {
                            transcript_speakers[tier_name].push(speaker_name);
                        }

                        var getParams = function (params) {
                            params.oldValue = $(this).text();
                            params.speakers = transcript_speakers[tier_name];
                            return params;
                        };

                        $(this).editable({
                            'title': 'Speaker (' + displays[tier_name] + ')',
                            'mode': 'popup',
                            'toggle': 'manual',
                            'placement': 'right',
                            'showbuttons': 'bottom',
                            'type': 'typeaheadjs',
                            'onblur': 'ignore',
                            'pk': $(this).closest('[data-tcuid]').attr('data-tcuid'),
                            'name': tier_name,
                            'params': getParams,
                            'url': Drupal.settings.basePath + 'tcu/up/speaker',
                            'success': function (response, newValue) {
                                if (response.status == 'success') {
                                    //change additional speaker names if called for
                                    for (var i=0; i<response.data.tcuids.length; i++) {
                                        $("*[data-speaker-display='" + tier_name + "']", $("*[data-tcuid='" + response.data.tcuids[i] + "']")).html(newValue).editable('setValue', newValue);
                                    }
                                    transcript_speakers[tier_name] = response.data.speakers;
                                }
                            },
                            'typeahead': [
                                {
                                    minLength: 1,
                                    highlight: true,
                                    hint: true,
                                    classNames: { //use what x-editable expects
                                        hint: 'tt-hint',
                                        input: 'tt-query',
                                        menu: 'tt-dropdown-menu',
                                        cursor: 'tt-is-under-cursor'
                                    }
                                },
                                {
                                    name: 'Name',
                                    source: speaker_names[tier_name]
                                }
                            ]
                        });
                        $(this).on('shown', function (e, editable) {
                            //make sure both .tt-hint and .tt-query have same styles; choose one:
                            //$('input', editable.container.$form).addClass('form-control');
                            $('input', editable.container.$form).removeClass('form-control');
                        });
                    }).click(
                        function (e) {
                            e.stopPropagation();
                            $(this).editable('toggle');
                        }
                    );
                });
        }
    }
})(jQuery);