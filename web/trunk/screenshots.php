<?php

	require_once 'site/base.inc.php';

	head( 'Screenshots - jMList' );
	topheading( 'screenshots' );


	open( 'div', array( 'id' => 'body' ) );

	h2( 'Live Screenshot' );

	$_jMList = txttag( 'span', array( 'class' => 'jmlist' ), 'jMList' );

	open( 'div', array( 'class' => 'margin' ) );
	open( 'table', array( 'border' => '0' ) );
	open( 'tr' );

	tag( 'td', array( 'align' => 'center' ),
		a(
			txttag( 'img', array(
				'border' => 0,
				'src' => 'demo/rclayderman-thumb.png',
				'alt' => 'Try it!'
			) ),
			'demo/rclayderman.html',
			'Try it!'
		)
	);

	tag( 'td', array( 'valign' => 'top' ),
		txttag( 'p', array(),
			"This is an example of output produced by $_jMList. The ".
			"stylesheet used was xhtml-collapsible, which generates a ".
			"tree listing in XHTML using JavaScript to make folders ".
			"collapsible like in a file manager. It still lacks tag ".
			"information (title, artist, etc)."
		).
		txttag( 'p', array(),
			a( "Here", 'demo/rclayderman.xml' ).
			" is the unstyled XML output produced by jMList."
		)
	);

	close( 'div' );

?>
