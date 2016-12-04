TRANSFORMING INCOMING TRANSCRIPTS WITH XSL

For transcripts to be ingested into the system, they must be uploaded as XML
to a file field of a node. Incoming XML is transformed using one of two methods.

The first method uses Saxonica's Saxon-9. Saxon 9 is an XSLT 2 processor,
which means that using this method, your XSL transformers can take full 
advantage of XSLT 2. We use the home edition, saxon9he.jar, although this 
is not necessary. Transcripts XSLT has not been well-tested with other 
versions of Saxon.

The disadvantage of this method is that Saxon-9 is called via PHP's exec(), 
which launches a Java VM to do each transformation. Not only is this
inefficient when multiple transforms are required, but also it may not be an
available option with some hosts.

The second method for transforming transcripts uses PHP's built-in
XSLT 1.0 capabilities with the libxslt extension. This means that transforms
can take place efficiently within PHP. The disadvantage of this method is 
that stylesheets are stuck in the dark ages of XSLT 1.0.

To configure the module, go to admin/config/user-interface/transcripts/transcripts_xslt
for your site, select XSLT 1.0 or XSLT 2.0, and then upload the transformers
that the site will use for different file formats. The XSL directory of this
module provides examples of XSL transformers. Files that end in "-1" use XSLT 1.0,
and the others use XSLT 2.0.

Incoming XML can be in any format, but the output must be very simple: 
a sequence of Time Coded Unit (<tcu>) tags within a root <tcus> tag. 
Each <tcu> tag should contain a <start> and an <end> tag. The optional 
<speakers> tag contains the speaker name in various transliteration formats. 
In the example below, the speaker name occurs in two user-defined formats, 
<ss_speaker_bod> (Tibetan) and <ss_speaker> (Default).

A <tcu> tag should also contain a set of user-defined <tiers>. The tier
label should be the same as the name of the field sent to the Solr index. Take 
a look at Drupal's schema.xml file to familiarize yourself with the Apache Solr 
Search Integration module's conventions for the naming of dynamic fields.

Example target:

<tcus>
   <tcu>
     <speakers>
        <ss_speaker_bod>ཟླ་སྒྲོན་ལགས</ss_speaker_bod>
        <ss_speaker>Dadron</ss_speaker>
     </speakers>
     <start>0.0</start>
     <end>3.7</end>
     <tiers>
        <ts_said>That dinner was interesting.</ts_said>
        <ts_meant>It wasn't my favorite.</ts_meant>
     </tiers>
   </tcu>
   <tcu>
     <speaker>Alice</speaker>
     <start>3.8</start>
     <end>6.1</end>
     <tiers>
        <ts_said>Yeah, it wasn't bad.</ts_said>
        <ts_meant>I'm not satisfied.</ts_meant>
     </tiers>
   </tcu>
</tcus>


Notes:

1. Start and end times must be given in seconds.

<< Last updated, 6 November 2016 >>
