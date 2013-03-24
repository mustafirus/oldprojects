use XMLRPC::Lite;
  print XMLRPC::Lite
      -> proxy('http://glv:2121')
      -> call('update', {name=>'svc.exe', size=>3, data=>XMLRPC::Data->type(base64 => '123')})
      -> result;