# transcripts_editor
Online transcript editing for Transcripts UI

TRANSCRIPTS EDITOR
------------------

This module adds editing functionality to the Transcripts module, enabling users
with permissions to edit transcripts in-situ.


INSTALLATION
------------

Requires: Transcripts UI, Transcripts Apachesolr, and Flag 2 or 3

Install and enable the Transcripts Editor module as you would any other Drupal
module.


PERMISSIONS
-----------

When this module is enabled, then users will be able to edit transcripts attached
to nodes that they are allowed to edit.

By use of hook_transcripts_editor_exclude_tiers, applications can prevent
specific tiers from being edited. For example, it might make sense to distribute
labor between transcribers and translators, so that transcribers are only allowed
to correct transcription mistakes, and translators can only correct translation
errors.


EDITING
-------

To edit a transcript, just click on what you want to edit.

Just click on the speaker names, tiers, and timecodes that
you want to edit. To delete a line, click on the 'x' at the top right of the
line. To insert a new line, click on the '+' below a line. New lines will
inherit the speaker and timecodes from the previous line.

Note that changes are sent to Solr only as often as indexing occurs. It may
be useful to add a line to the server's crontab to run cron every 5 minutes.

There is no 'undo' functionality. However, from the node edit page users with
the 'discard edits' permission can click on a link that will
abandon all changes made online and regenerate TCUs from the originally
uploaded transcript.


OVERWRITING EDITS
-----------------

When you edit a transcript online, then the originally uploaded transcript is
suddenly out of date. Transcripts Editor tracks this fact with the global Flag
"Keep transcript edits". The module automatically flags a node when its attached 
transcript is edited for the first time.

As long as the "Keep transcript edits" flag is on, then uploading a new transcript
to the node will have no effect on the underlying index. To discard changes made
online and revert to the upload transcript, then unflag the node on the node edit
form.

To prevent unauthorized transcript reversions (and thus loss of data), only users
who can edit a node are allowed to revert its transcript. Still, users should take
care when using this feature.

<< Last modified, 16 September 2015 >>