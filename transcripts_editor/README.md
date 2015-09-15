# transcripts_editor
Online transcript editing for Transcripts UI

TRANSCRIPTS EDITOR
------------------

This module adds editing functionality to the Transcripts module, enabling users
with permissions to edit transcripts in-situ.


INSTALLATION
------------

Requires: Transcripts, Transcripts Node, and Flag

Install and enable the Transcripts Editor module as you would any other Drupal
module.


PERMISSIONS
-----------

By use of hook_transcripts_editor_exclude_tiers, applications can prevent
specific tiers from being edited. For example, it might make sense to distribute
labor between transcribers and translators, so that transcribers are only allowed
to correct transcription mistakes, and translators can only correct translation
errors.

To be able to edit transcripts, users need the 'Edit transcripts and timecodes'
permission. Granting this permission to a user will give her the ability to
edit any transcript belonging to a node that she can edit. She won't be able to
edit transcripts belonging to nodes that she cannot edit.

So, to make editing transcripts part of editing nodes, just give the permission
to all authenticated users. To restrict transcript editing further, create a
dedicated transcript editing role.


EDITING
-------

To edit, click on the 'Transcript' tab for the node (node/%/transcript). Click
on the 'pencil' icon under the video. (If you don't see a pencil icon, then
you don't have permission to edit this transcript.)

Once in edit mode, just click on the speaker names, tiers, and timecodes that
you want to edit. To delete a line, click on the 'x' at the top right of the
line. To insert a new line, click on the '+' below a line. New lines will
inherit the speaker and timecodes from the previous line.

Changes are automatically saved to Drupal every ten seconds. So if you make
a change don't immediately leave or refresh the page. In general you can just
leave the editor open and all changes will be saved.

Note that changes are sent to Solr only as often as indexing occurs. It may
be useful to add a line to the server's crontab to run cron every 5 minutes.

There is no 'undo' functionality. However, from the node edit page users with
the 'Abandon transcript changes' permission can click on a link that will
abandon all changes made online and regenerate TCUs from the originally
uploaded transcript.


TRANSCRIPTS EDITOR DISABLE

Requires: Transcripts Editor and Flag

Use this module to enable certain roles to freeze the editing of specific
transcripts. For example, you may want to disable online transcript editing
when a transcript is downloaded for offline editing.

To configure this module, visit admin/structure/flags and assign some user
roles the ability to flag and unflag content with transcripts_editor_disable.

<< Last modified, 27 July 2014 >>