###############################################
#
# Automated quaerobot
#
# Uses LWP to submit form request
# Daniel Whiteson 6/03
# danielw@fnal.gov

# run like

# perl quaerobot.pl <signalfile> <tag> 


require LWP::UserAgent;
use HTTP::Request::Common;
use HTTP::Cookies;


# define server and form
    my $ctServer = "http://hep.uchicago.edu";
    my $ctAction = "/cgi-bin/quaero.py";


# get new agent
    my $ua = LWP::UserAgent->new();

$fname_in = $ARGV[0];
$tag = $ARGV[1];
$quiet = $ARGV[2];

# print stuff
unless ($quiet){
print "*************************************\n* Automated Quaerobot\n* Submits request to: $ctServer.$ctAction\n*************************************\n";
}


if ($fname_in eq "")
{
    
    print "\n\nUsage: perl quaerobot.pl <signal-file>\n";
    exit(-1);
}
unless ($quiet){
print " Using file: $fname_in\n";

}
# build request

my $req = HTTP::Request::Common::POST($ctServer.$ctAction, 
				      Content_Type => 'form-data',
				      Content => [
						  d0 => "1",
						  tev2 => "1",
						  
# For pythia input
#						  inputType_tev2 => "PythiaInput",
#						  PythiaInput_tev2 => "msel=6",
						  
# For custom input
						  
						  inputType_tev2 => "SignalFile",
						  signalFile_tev2 => [$fname_in],


# Backgrounds
# Lep
						  "qq" => "qq",
						  "e+e-" => "e+e-",
						  "l+l-" => "l+l-",
						  "1ph" => "1ph",
						  "4f" => "4f",
						  "WW" => "WW",
						  "multi-ph" => "multi-ph",
						  "2ph" => "2ph",
#Tev

						  jj => "jj",
						  pj => "pj",
						  pp => "pp",
						  pyth_w => "pyth_w",
						  pyth_z => "pyth_z",
						  pyth_vv => "pyth_vv",
						  pyth_tt => "pyth_tt",
# Details about requestor						  
						  
						  RequestorName => "Daniel Quaerobot",
						  RequestorInstitution => "LBL",
						  RequestorEmail => "danielw\@fnal.gov",
						  ModelDescription => "$tag",
						  Password => "ular"
						  ]       
			      );

#submit request

my $r = $ua->request($req);

#provide output

unless ($quiet){
print "\n\n---------- RESULTS ---------\n";
print "\nRun with error?: ";
print $r->is_error;

print "\nError: ";
print $r->message; 
   
print "\n ***** FORM OUTPUT ********\n";
print $r->content;
}
($reqID) = ($r->content =~ /Quaero\srequest\s\#(\d+)/);

print "Request $reqID\n";



















