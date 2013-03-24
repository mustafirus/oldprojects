package MyStash;

use Data::Dumper;
use Template::Stash;

BEGIN {
#  @MyStash::ISA = qw( Template::Stash );
  use base 'Template::Stash';
  @LOSTVARS = ();
  $LOSTVARS = \@LOSTVARS;
  $curfile='';
  %var2file;
}
#$ERRLOG = \*STDERR;

sub DESTROY {
  # no op
  1;
}

#------------------------------------------------------------------------
# undefined($ident, $args)
#
# Method called when a get() returns an undefined value.  Can be redefined
# in a subclass to implement alternate handling.
#------------------------------------------------------------------------

sub undefined {
    my ($self, $ident, $args) = @_;
    my $name = join('.', grep { !/0/ } @$ident);
#    print "!!Undef:$name ", caller(0), "\n" if $name;
    
    push @$LOSTVARS, $name unless grep { /^$name$/ } @$LOSTVARS or !$name; 
    return '';
}

sub get {
    my ($self, $ident, $args) = @_;
    my $name = join('.', grep { !/0/ } @$ident);
    $res = $self->SUPER::get($ident, $args);
#    push @$LOSTVARS, $name unless grep { /^$name$/ } @$LOSTVARS or !$name or $res; 
#    printf "!!! $name: $res\n";
    return $res;
}

# catch missing method calls here so perl doesn't barf 
# trying to load *.al files 
sub AUTOLOAD {

  my ($self, @args) = @_;
  my @c             = caller(0);
  my $auto	    = $AUTOLOAD;

  $auto =~ s/.*:://;
  $self =~ s/=.*//;

  die "Can't locate object method \"$auto\"" .
      " via package \"$self\" at $c[1] line $c[2]\n";
}

1;

__END__


