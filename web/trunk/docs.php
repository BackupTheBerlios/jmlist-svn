<?php

	require_once 'site/base.inc.php';

	head( 'Documentation - jMList' );
	topheading( 'docs' );


	open( 'div', array( 'id' => 'body' ) );

	$_jMList = txttag( 'span', array( 'class' => 'jmlist' ), 'jMList' );


	h2( 'User Documentation' );

	tag( 'p', array(), a( "jMList Manual", 'manual-html/' ) );

	tag( 'p', array(),
		"This documentation is intended for users of $_jMList. It is focused ".
		"on how to use this utility."
	);


	h2( 'Developer Documentation' );

	tag( 'p', array(), a( "jMList Developer's Reference", 'dev-manual/html/' ) );

	tag( 'p', array(),
		"This documentation provides information about all functions, ".
		"types and enumerations used on $_jMList source code. It is useful ".
		"only for developers."
	);

	tag( 'p', array(),
		"It was generated from the commented source code using ".
		a( "Doxygen", 'http://www.doxygen.org/' ). "."
	);


?>
