var TranscriptTimeUtil = {
    formatMs: function (t) {
        var asfloat = parseFloat(t);
        var mins = Math.floor(asfloat / 60);
        var secs = asfloat % 60;
        var time = (mins < 10 ? "0" + mins.toString() : mins.toString()) + ":" + (secs < 10 ? "0" + secs.toFixed(3) : secs.toFixed(3));
        return time;
    },
    getRange: function (t) {
        var time = parseFloat(t);
        var min = time - 3;
        if (min < 0) min = 0;
        var max = time + 3;

        return {
            time: time.toFixed(3),
            min: min.toFixed(3),
            max: max.toFixed(3)
        };
    }
};

(function ($) {
    Drupal.behaviors.transcriptsEditor = {
        attach: function (context, settings) {
            $('[data-transcripts-role=transcript]', context)
                .addBack('[data-transcripts-role=transcript]')
                .once('transcripts-editor')
                .each(function () {
                    var $transcript = $(this);
                    var alreadyEditing = false;

                    $('a[href$="' + settings.transcripts_editor.edit_transcript_link + '"]')
                        .addClass('transcript-edit')
                        .click(function (e) {
                            e.preventDefault();
                            if (!alreadyEditing) {
                                alreadyEditing = true;
                                activateEditing($transcript);
                                $(this).closest('ul').find('li').removeClass('active').find('a').removeClass('active');
                                $(this).addClass('active').closest('li').addClass('active');
                            }
                        });
                });
        }
    }
    function activateEditing($transcript) {
        var trid = $transcript.attr('data-transcripts-id');

        /* language tiers */
        var exclude = $.map(Drupal.settings.transcripts_editor.exclude,
            function (val, i) {
                return '[data-tier=' + val + ']';
            }
        );
        $('.tier', $transcript).not(exclude.join(',')).each(function () {
            $(this).editable({
                'mode': 'inline',
                'toggle': 'click',
                'showbuttons': 'bottom',
                'type': 'textarea',
                'rows': 3,
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

        /* speaker names */
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

        $('.speaker-display', $transcript).each(function () {
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
                'mode': 'inline',
                'toggle': 'manual',
                'showbuttons': 'right',
                'type': 'typeaheadjs',
                'pk': $(this).closest('[data-tcuid]').attr('data-tcuid'),
                'name': tier_name,
                'params': getParams,
                'url': Drupal.settings.basePath + 'tcu/up/speaker',
                'success': function (response, newValue) {
                    switch (response.status) {
                        case 'success':
                            //change additional speaker names if called for
                            for (var i = 0; i < response.data.tcuids.length; i++) {
                                $("*[data-speaker-display='" + tier_name + "']", $("*[data-tcuid='" + response.data.tcuids[i] + "']")).html(newValue).editable('setValue', newValue);
                            }
                            transcript_speakers[tier_name] = response.data.speakers;
                            break;
                        case 'error':
                            return response.message;
                            break;
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
        }).click(function (e) {
            e.stopPropagation();
            $(this).editable('toggle');
        });

        /* timecodes */
        $('.play-button', $transcript).each(function () {
            var $that = $(this);

            var $tcu = $(this).closest('[data-tcuid]');
            var begin = TranscriptTimeUtil.getRange($tcu.attr('data-begin'));

            var getTimecodes = function (params) {
                params.begin = $('input[name=beginInput]', $tcu).val();
                params.end = $('input[name=endInput]', $tcu).val();
                return params;
            };

            $(this).editable({
                'title': 'Adjust timecodes',
                'mode': 'inline',
                'toggle': 'manual',
                'onblur': 'ignore',
                'showbuttons': 'bottom',
                'type': 'range',
                'name': 'time',
                'value': begin.time,
                'min': begin.min,
                'max': begin.max,
                'pk': $tcu.attr('data-tcuid'),
                'params': getTimecodes,
                'savenochange': true, //because of the inserted end range input element
                'tpl': "<input name='beginInput' type='range' step='0.001' oninput='beginOutput.value=TranscriptTimeUtil.formatMs(beginInput.value); Drupal.settings.scrollingTranscript[\"" + trid + "\"].setCurrentTime(parseFloat(beginInput.value));''>",
                'display': function (value, sourceData) {
                    var asfloat = parseFloat(value);
                    var mins = Math.floor(asfloat / 60);
                    var secs = Math.floor(asfloat % 60);
                    var time = mins.toString() + ":" + (secs < 10 ? "0" + secs.toString() : secs.toString());
                    $('.play-tcu', this).html("<span class='glyphicon glyphicon-play'/> " + time);
                    return false;
                },
                'validate': function (value) {
                    var t1 = parseFloat($('input[name=beginInput]', $tcu).val());
                    var t2 = parseFloat($('input[name=endInput]', $tcu).val());
                    if (!(t1 < t2)) {
                        return "Start time must precede end time.";
                    }
                },
                'url': Drupal.settings.basePath + 'tcu/up/times',
                'success': function (response, newValue) {
                    switch (response.status) {
                        case 'success':
                            var t1 = TranscriptTimeUtil.getRange(response.data.begin);
                            var t2 = TranscriptTimeUtil.getRange(response.data.end);
                            $tcu.attr({
                                'data-begin': t1.time,
                                'data-end': t2.time
                            });
                            /* refresh begin time */
                            $(this).editable('setValue', t1.time);
                            $(this).editable('option', 'min', t1.min);
                            $(this).editable('option', 'max', t1.max);
                            $(this).editable('option', 'tpl', "<input name='beginInput' type='range' step='0.001' oninput='beginOutput.value=TranscriptTimeUtil.formatMs(beginInput.value); Drupal.settings.scrollingTranscript[\"" + trid + "\"].setCurrentTime(parseFloat(beginInput.value));'>");
                            break;
                        case 'error':
                            return response.message;
                            break;
                    }
                }
            }).after($("<div class='edit-times'><span class='glyphicon glyphicon-edit'/></div>").click(
                function (e) {
                    e.stopPropagation();
                    $(this).hide(); //hide icon
                    $('.play-button', $tcu).editable('toggle');
                }
            ));

            $(this).on('shown', function (e, editable) {
                var begin = TranscriptTimeUtil.getRange($tcu.attr('data-begin'));
                var end = TranscriptTimeUtil.getRange($tcu.attr('data-end'));

                $('.editable-input', editable.container.$form)
                    .after($("<div class='end-range'><input name='endInput' type='range' min='" + end.min + "' max='" + end.max + "' step='0.001' oninput='endOutput.value=TranscriptTimeUtil.formatMs(endInput.value); Drupal.settings.scrollingTranscript[\"" + trid + "\"].setCurrentTime(parseFloat(endInput.value));''></div>"))
                    .after($("<div class='range-display'><button class='btn btn-default btn-icon play-range' type='button'><span class='glyphicon glyphicon-play'/> <output name='beginOutput'>"
                        + TranscriptTimeUtil.formatMs(begin.time) + "</output> -&gt; <output name='endOutput'>"
                        + TranscriptTimeUtil.formatMs(end.time) + "</output></button></div>"))
                    .addClass('begin-range')
                    .find('input').removeClass('form-control');

                $('.play-range', $tcu).click(function (e) {
                    var t1 = parseFloat($('input[name=beginInput]', $tcu).val());
                    var t2 = parseFloat($('input[name=endInput]', $tcu).val());
                    if (t2 > t1) {
                        var scroller = Drupal.settings.scrollingTranscript[trid];
                        scroller.playOne($tcu, true, t1, t2);
                    }
                });
            }).on('hidden', function (e, reason) {
                $('div.edit-times', $tcu).show(); //show icon
            });
        });
        $transcript.prepend("<div class='editing-message'>Transcript editing is active.</div>").addClass('editing-active');
        $('body').addClass('is-transcript-editing');
    }

})(jQuery);