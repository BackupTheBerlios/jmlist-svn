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
	tag( 'li', array(),
		a( 'popt', 'ftp://ftp.rpm.org/pub/rpm/dist/rpm-4.1.x/' ).
		" (temporary dependency, it'll soon be included on the package)"
	);
	tag( 'li', array(), a( 'GLib', 'http://www.gtk.org/' ) );
	tag( 'li', array(), a( 'LibXML2', 'http://xmlsoft.org/' ) );
	tag( 'li', array(), a( 'ID3Lib', 'http://id3lib.sourceforge.net/' ) );
	close();


	h2( 'Binary/Source Packages' );

	tag( 'p', array(),
		"Since $_jMList is still in an earlier stage of development, ".
		"there aren't any packages released. However, if you want to test ".
		"it, feel free to checkout the latest repository snapshot."
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
