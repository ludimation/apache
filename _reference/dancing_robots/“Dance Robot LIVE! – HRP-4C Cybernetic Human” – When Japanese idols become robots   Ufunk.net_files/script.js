jQuery.noConflict();

// masonry code 
jQuery(document).ready(function() {
	
	// Grid jQuery plugin: http://desandro.com/resources/jquery-masonry/
	jQuery('#sort').masonry({ 
		columnWidth: 320,
		gutterWidth: 27,
		animate: false,
		itemSelector: '.box'
	}, 
	function() { jQuery(this).css({
		margin: '0px'
		});
	});
		
});

jQuery(function(){
	jQuery('span.view-large').hide();
	myOp = 1;
	
	jQuery(window).load(function(){
	
		
		
		
		jQuery('#sort').masonry('reload');
		
		// OVERLAY mini
		jQuery('.descMini').hover(function()
		{
			jQuery(this).find('.descriptionMini').slideDown("fast");
		},function()
		{
			jQuery(this).find('.descriptionMini').slideUp("fast");
		});
		
		// OVERLAY mini
		jQuery('.descMini4').hover(function()
		{
			jQuery(this).find('.descriptionMini4').slideDown("fast");
		},function()
		{
			jQuery(this).find('.descriptionMini4').slideUp("fast");
		});
		
		
		// OVERLAY big
		jQuery('.descMini2').hover(function()
		{
			jQuery(this).find('.descriptionMini2').slideDown("fast");
		},function()
		{
			jQuery(this).find('.descriptionMini2').slideUp("fast");
		});
		
		// OVERLAY big 2
		jQuery('.descMini3').hover(function()
		{
			jQuery(this).find('.descriptionMini3').animate({opacity: 0.9}, 50, function() {});
			jQuery(this).animate({opacity: 0.8}, 50, function() {});
		},function()
		{
			jQuery(this).find('.descriptionMini3').animate({opacity: 0.6}, 50, function() {});
			jQuery(this).animate({opacity: 1}, 50, function() {});
		});
		
		// OVERLAY menubox
		jQuery('.menuboxitem').hover(function()
		{
			jQuery(this).removeClass('menuboxitem');
			jQuery(this).animate({opacity: 1}, 50, function() {});
		},function()
		{
			jQuery(this).animate({opacity: 0.4}, 50, function() {}).addClass('menuboxitem');;	
		});
		
		
		
		
		
		// MouseOver Events
		
		jQuery('.box').hover(function(){
			jQuery('img', this).fadeTo("fast", 0.7).addClass('box-hover');
			jQuery('span.view-large', this).fadeTo("fast", 1)},
			function(){
			jQuery('img', this).fadeTo("fast", myOp).removeClass('box-hover');
			jQuery('span.view-large', this).fadeTo("fast", 0)
		});
		
		// Colorbox
		jQuery("a[rel='gallery']").colorbox();
		
	});
});
