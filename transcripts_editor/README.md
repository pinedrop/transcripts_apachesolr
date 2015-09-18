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

Enabling this module allows users to edit transcripts attached to nodes that they 
have the permission to edit.

The hooks hook_transcripts_editor_disabled and hook_transcripts_editor_exclude_tiers
allow specific nodes or data tiers to be excluded from editing. For example,
it might make sense to distribute labor between transcribers and translators, so 
that transcribers are only allowed to correct transcription mistakes, and translators 
only to correct translation errors.


EDITING
-------

To edit a transcript, click on the tier that you want to edit.

Note that transcript edits are sent to Solr only as often as indexing occurs. 
It may be useful to add a line to the server's crontab to run cron every 5 minutes,
so that one's edits are visible to others more quickly.

It may also be useful to occasionally refresh the page, in case other people are
editing the transcript at the same time.

There is no undo functionality. However, it is possible to overwrite edits made
online (see below).


OVERWRITING EDITS
-----------------

When you edit a transcript online, then the originally uploaded transcript is
suddenly out of date. Transcripts Editor tracks this fact with the global Flag
"Keep transcript edits". The module automatically flags a node when its attached 
transcript is edited for the first time.

As long as the "Keep transcript edits" flag is on, then one is prevented from
deleting or replacing the uploaded transcript. To discard changes made online 
and revert to the upload transcript, then unflag the node on the node edit
form.

On the flip side, one might want to disable online transcript editing when a
transcript has been "checked out" for offline editing. To implement such a feature,
hook_transcripts_editor_disabled would probably come in handy.


<< Last modified, 16 September 2015 >>