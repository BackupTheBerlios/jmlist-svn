<?php

	require_once 'site/base.inc.php';

	head( 'jMList' );
	topheading( 'home' );

	open( 'div', array( 'id' => 'body' ) );

	h2( 'Overview' );

	$_jMList = txttag( 'span', array( 'class' => 'jmlist' ), 'jMList' );
	$_XML = acronym( 'XML', 'eXtensible Markup Language' );
	$_HTML = acronym( 'HTML', 'HyperText Markup Language' );
	$_XSL = acronym( 'XSL', 'XML Stylesheet' );
	$_RDF = acronym( 'RDF', 'Resource Description Framework' );
	$_RSS = acronym( 'RSS', 'RDF Site Summary' );

	tag( 'p', array(),
		"$_jMList is a small utility that scans one or more given ".
		"directories for media files (currently only audio files) and ".
		"generates a $_XML listing of the whole directory tree with ".
		"information about each media file such as filesize, song length, ".
		"title, author, album, etc. A $_XSL stylesheet can be applied to ".
		"the output to transform into a different output, like tabular or ".
		"tree listings in plain text, $_XML, $_HTML, $_RDF, $_RSS or any ".
		"other $_XML format."
	);

	$_MP3 = acronym( 'MP3', 'MPEG Layer-3' );

	tag( 'p', array(),
		"$_jMList is still in its earlier stages of developement, but it ".
		"already supports identification of $_MP3 files and is capable of ".
		"extracting meta-information from both ID3 and ID3v2 tags."
	);

	tag( 'p', array(),
		"$_jMList is released under the ".a( 'GNU', 'http://www.gnu.org/' )." ".
		a( 'General Public License', 'http://www.gnu.org/licenses/gpl.html' ).
		"."
	);

	h2( 'Features' );

	tag( 'p', array(),
		"The following features are currently supported by $_jMList:"
	);

	open( 'ol', array() );
	tag( 'li', array(), "Reading of MPEG Layer 1, 2 and 3 ($_MP3) formats." );
	tag( 'li', array(), "Reading of ID3 v1 and v2 tags." );
	tag( 'li', array(), "Full Unicode support." );
	tag( 'li', array(), "Parsing data in UTF-8 and ISO8859-1 encondings from ID3 tags." );
	close();

	tag( 'p', array(),
		"The following features are planned to be supported:"
	);

	open( 'ol' );
	tag( 'li', array(), "Internally transform output through $_XSL." );
	tag( 'li', array(), "Detect and parse data from tags based on an user-provided list of possible character encodings." );
	tag( 'li', array(), "Properly support files > 2GiB." );
	tag( 'li', array(), "National Language (NLS, i18n) support" );
	tag( 'li', array(), "Reading of most audio (MPC, OGG, FLAC) formats." );
	tag( 'li', array(), "Reading of most video (AVI, MPEG, OGM, MKV) formats." );
	close();

?>
