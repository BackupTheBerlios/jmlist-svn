<?php

	require_once 'site/base.inc.php';

	head( 'jMList' );
	topheading( 'home' );

	open( 'div', array( 'id' => 'body' ) );

	h2( 'Overview' );

	$_jMList = txttag( 'span', array( 'class' => 'jmlist' ), 'jMList' );
	$_XML = acronym( 'XML', 'eXtensible Markup Language' );
	$_HTML = acronym( 'HTML', 'HyperText Markup Language' );
	$_XSL = acronym( 'XSL', 'eXtensible Stylesheet Language' );
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
	$_FLAC = acronym( 'FLAC', 'Free Lossless Audio Codec' );

	tag( 'p', array(),
		"$_jMList is still in its earlier stages of developement, but it ".
		"already supports identification of $_MP3 and OGG files and is capable ".
		"of extracting meta-information from ID3, ID3v2 and Vorbis tags."
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
	tag( 'li', array(), "Internally transform output through $_XSL." );
	tag( 'li', array(), "Parsing information from MPEG Layer 1, 2 and 3 ($_MP3) files, both ID3 v1 and v2 tags." );
	tag( 'li', array(), "Parsing information from Ogg Vorbis (OGG) files." );
	tag( 'li', array(), "Full Unicode support." );
	tag( 'li', array(), "Parsing data in UTF-8 and ISO8859-1 encondings from ID3 tags." );
	close();

	tag( 'p', array(),
		"The following features are planned to be supported:"
	);

	$_NLS = acronym( 'NLS', 'National Language Support' );
	$_i18n = acronym( 'i18n', 'internationalization' );

	open( 'ol' );
	tag( 'li', array(), "Detect and parse data from tags based on an user-provided list of possible character encodings." );
	tag( 'li', array(), "Properly support files > 2GiB." );
	tag( 'li', array(), "National Language ($_NLS, $_i18n) support" );
	tag( 'li', array(), "Reading of most common audio (MPC, $_FLAC, APE) formats." );
	#tag( 'li', array(), "Reading of most common video (AVI, MPEG, OGM, MKV) formats." );
	close();

?>
