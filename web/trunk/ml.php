<?php

	require_once 'site/base.inc.php';

	head( 'Mailing Lists - jMList' );
	topheading( 'ml' );


	open( 'div', array( 'id' => 'body' ) );

	h2( 'General Discussion' );

	$_jMList = txttag( 'span', array( 'class' => 'jmlist' ), 'jMList' );

	tag( 'p', array(),
		"If you have questions about $_jMList, need support or just want to ".
		"talk about its development, we invite you to join the ".
		a( "general discussion mailing list", 'http://lists.berlios.de/mailman/listinfo/jmlist-users' )."."
	);

?>
