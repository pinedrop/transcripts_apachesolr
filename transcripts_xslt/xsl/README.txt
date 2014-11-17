TRANSFORMING INCOMING TRANSCRIPTS WITH XSL

For transcripts to be ingested into the system, they must be uploaded as XML
to a file field of a node. Incoming XML is transformed by invoking Saxon 9 from
the command line using exec(). Saxon 9 is an XSLT 2 processor which means that 
your XSL transformers can take full advantage of XSLT 2. We use the home edition, 
saxon9he.jar, although this is not necessary. Transcripts has not been well-
tested with other versions of Saxon.

This directory provides examples of XSL transformers. Incoming XML can be in any 
format, but the output must be very simple: a sequence of Time Coded Unit (<tcu>) 
tags within a root <tcus> tag. Each <tcu> tag contains obligatory data such as 
<speaker>, <start> and <end>, and then a set of user-defined <tiers>. The tier 
label should be the same as the name of the field sent to the Solr index. Take 
a look at Drupal's schema.xml file to familiarize yourself with the Apache Solr 
Search Integration module's conventions for the naming of dynamic fields.


Example target:

<tcus>
   <tcu>
     <speaker>Richard</speaker>
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


<< Last updated, 8 May 2013 >>
