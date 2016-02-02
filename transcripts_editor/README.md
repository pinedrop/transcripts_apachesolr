TRANSCRIPTS EDITOR
------------------

This module adds editing functionality to the Transcripts module, enabling users
with permissions to edit transcripts in-situ.


INSTALLATION
------------

Requires: Transcripts UI, Transcripts Apachesolr, and Flag 2 or 3. Also requires
Typeahead, a module contained within [Drupal KMaps Modules](https://github.com/shanti-uva/drupal_kmaps_modules).

Install and enable the Transcripts Editor module as you would any other Drupal
module.

For the best experience, install Apachesolr Realtime with the following patch:

  https://www.drupal.org/node/2217823
   
Then, check "Time Code Unit" under the "Real Time Indexing" settings on your
Apache Solr Search configuration page (admin/config/search/apachesolr). This
way, all transcript edits will be sent to the index immediately, ensuring that
your changes will be immediately visible to others, and to yourself if you
switch from edit to view mode.


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

To edit a data tier (for example, a transcription or translation), click on the
text that you want to edit. The text becomes editable; save or cancel when finished.

To edit a speaker name, click on the speaker name, change it, and then save or cancel.
Editing speaker names works as follows. Suppose there are two speakers for the transcript,
'Billy' and 'Tommy'. If a line is marked as being spoken by 'Billy', and you change it 
to 'Tommy', then it will change that line, but other lines marked 'Billy' will not be
changed. In other words, it is assumed that a mistake was previously made in identifying
the line as being spoken by 'Billy'.

On the other hand, if a line is marked as being spoken by 'Billy', and you change it to
'Mr Billy', then it will change all instances of 'Billy' to 'Mr Billy'. In other words,
it is assumed that you just want to change the way the speaker name is recorded, and
that your change should affect the entire transcript.

It is not yet possible to add a new speaker name, for example to attribute an existing
line in this transcript to a third, previously unnoticed speaker named 'Freddy'.

Note that transcript edits are sent to Solr only as often as indexing occurs. 
It may be useful to add a line to the server's crontab to run cron every 5 minutes,
so that one's edits are visible to others more quickly.

It may also be useful to occasionally refresh the page, in case other people are
editing the transcript at the same time.

There is no undo functionality. However, it is possible to overwrite edits made
online (see below).


OVERWRITING EDITS
-----------------

When you edit a transcript online, the originally uploaded transcript is
suddenly out of date. Transcripts Editor tracks this fact with the global flag
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