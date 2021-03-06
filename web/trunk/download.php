<?php

	require_once 'site/base.inc.php';

	head( 'Download - jMList' );
	topheading( 'download' );


	open( 'div', array( 'id' => 'body' ) );

	$_jMList = txttag( 'span', array( 'class' => 'jmlist' ), 'jMList' );


	h2( 'Requirements' );

	tag( 'p', array(),
		"The following libraries are required in order to use or compile ".
		"$_jMList:"
	);

	open( 'ul', array() );
	tag( 'li', array(), a( 'GLib', 'http://www.gtk.org/' ) . " 2.4.0 or higher (required)" );
	tag( 'li', array(), a( 'LibXML', 'http://xmlsoft.org/' ) . " 2.6.0 or higher (require)" );
	tag( 'li', array(), a( 'LibXSLT', 'http://xmlsoft.org/XSLT/' ) . " 1.1.0 or higher (required)" );
	tag( 'li', array(), a( 'ID3Lib', 'http://id3lib.sourceforge.net/' ) . " 3.8.0 or higher (required)" );
	tag( 'li', array(), a( 'OggVorbis', 'http://www.xiph.org/ogg/vorbis/' ) . " 1.1.0 or higher (optional, for OggVorbis support)" );
	close();


	h2( 'Source Packages' );

	h3( 'Version 0.1.0 (beta)' );

	tag( 'p', array(),
		a( 'jmlist-0.1.0.tar.gz', 'http://download.berlios.de/jmlist/jmlist-0.1.0.tar.gz' ). " (initial release) - ".
		a( 'Release Notes', 'http://developer.berlios.de/project/shownotes.php?release_id=5187' )
	);


	h2( 'Repository Access' );

	tag( 'p', array(),
		"$_jMList is hosted on a " .
		a( 'Subversion', 'http://subversion.tigris.org/' )." repository. ".
		"You can checkout it from " . a( 'svn://svn.berlios.de/jmlist/source/trunk' )." ".
		"or browse it at ".a( 'http://svn.berlios.de/viewcvs/jmlist/source/trunk' )."."
	);

	tag( 'p', array(),
		"In order to compile from the repository, you will need the following ".
		"additional software:"
	);

	open( 'ul', array() );
	tag( 'li', array(), a( 'autoconf', 'http://www.gnu.org/software/autoconf/' ) . " 2.59 or higher");
	tag( 'li', array(), a( 'automake', 'http://www.gnu.org/software/automake/' ) . " 1.8 or higher" );
	close();

	tag( 'p', array(),
		"After checking-out the trunk tree, run ".
		txttag( 'code', array(), './configure.svn' ). " on the working copy ".
		"directory in order to generate support files needed to configure and ".
		"compile $_jMList. After that, proceed as usual with ".
		txttag( 'code', array(), './configure --help' ). ", ".
		txttag( 'code', array(), './configure' ). ", ".
		txttag( 'code', array(), 'make' ). " and ".
		txttag( 'code', array(), 'make install' ). "." 
	);

?>
