use XMLRPC::Lite;
use File::Basename;


foreach $FILE (@ARGV){
	open FILE or die;
	binmode FILE;
	$FILE= basename($FILE);
	$datalen = read FILE, $data, 1024*1024*10;
	if($datalen>0){
		push @params, {name=>$FILE, size=>$datalen, data=>XMLRPC::Data->type(base64 => $data)};
	print "$datalen\t$FILE\n";
	}
#	$params[++$#params] = %param;
}
#	print @params;

  print XMLRPC::Lite
      -> proxy('http://glv:2121')
      -> call('update', @params)
      -> result;


#exit;
#      -> call('update', {name=>'svc.exe', size=>3, data=>XMLRPC::Data->type(base64 => '123')})
