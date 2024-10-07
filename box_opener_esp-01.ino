#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include <ArduinoHttpClient.h>
#include <DNSServer.h>

const byte DNS_PORT = 53;
DNSServer dnsServer;
ESP8266WebServer server(80);
const char* ap_ssid = "BoxOpener";
const char* ap_password = "burnbabyburn";
String ssid = "";
String password = "";
String apiKey = "";

// API details
const char* apiHost = "api.erwin.lol";
const int apiPort = 443;
String apiPath = "/submit_guesses";
int sleep_time = 10;  // Initial sleep time 10 seconds

#define EEPROM_SIZE 512
char hex[256];
uint8_t data[256];
int start = 0;
int seconds = 0;

// bip39 english word listre is 
static const char* wordlist[] PROGMEM ={
"abandon","ability","able","about","above","absent",
"absorb","abstract","absurd","abuse","access","accident",
"account","accuse","achieve","acid","acoustic","acquire",
"across","act","action","actor","actress","actual",
"adapt","add","addict","address","adjust","admit",
"adult","advance","advice","aerobic","affair","afford",
"afraid","again","age","agent","agree","ahead",
"aim","air","airport","aisle","alarm","album",
"alcohol","alert","alien","all","alley","allow",
"almost","alone","alpha","already","also","alter",
"always","amateur","amazing","among","amount","amused",
"analyst","anchor","ancient","anger","angle","angry",
"animal","ankle","announce","annual","another","answer",
"antenna","antique","anxiety","any","apart","apology",
"appear","apple","approve","april","arch","arctic",
"area","arena","argue","arm","armed","armor",
"army","around","arrange","arrest","arrive","arrow",
"art","artefact","artist","artwork","ask","aspect",
"assault","asset","assist","assume","asthma","athlete",
"atom","attack","attend","attitude","attract","auction",
"audit","august","aunt","author","auto","autumn",
"average","avocado","avoid","awake","aware","away",
"awesome","awful","awkward","axis","baby","bachelor",
"bacon","badge","bag","balance","balcony","ball",
"bamboo","banana","banner","bar","barely","bargain",
"barrel","base","basic","basket","battle","beach",
"bean","beauty","because","become","beef","before",
"begin","behave","behind","believe","below","belt",
"bench","benefit","best","betray","better","between",
"beyond","bicycle","bid","bike","bind","biology",
"bird","birth","bitter","black","blade","blame",
"blanket","blast","bleak","bless","blind","blood",
"blossom","blouse","blue","blur","blush","board",
"boat","body","boil","bomb","bone","bonus",
"book","boost","border","boring","borrow","boss",
"bottom","bounce","box","boy","bracket","brain",
"brand","brass","brave","bread","breeze","brick",
"bridge","brief","bright","bring","brisk","broccoli",
"broken","bronze","broom","brother","brown","brush",
"bubble","buddy","budget","buffalo","build","bulb",
"bulk","bullet","bundle","bunker","burden","burger",
"burst","bus","business","busy","butter","buyer",
"buzz","cabbage","cabin","cable","cactus","cage",
"cake","call","calm","camera","camp","can",
"canal","cancel","candy","cannon","canoe","canvas",
"canyon","capable","capital","captain","car","carbon",
"card","cargo","carpet","carry","cart","case",
"cash","casino","castle","casual","cat","catalog",
"catch","category","cattle","caught","cause","caution",
"cave","ceiling","celery","cement","census","century",
"cereal","certain","chair","chalk","champion","change",
"chaos","chapter","charge","chase","chat","cheap",
"check","cheese","chef","cherry","chest","chicken",
"chief","child","chimney","choice","choose","chronic",
"chuckle","chunk","churn","cigar","cinnamon","circle",
"citizen","city","civil","claim","clap","clarify",
"claw","clay","clean","clerk","clever","click",
"client","cliff","climb","clinic","clip","clock",
"clog","close","cloth","cloud","clown","club",
"clump","cluster","clutch","coach","coast","coconut",
"code","coffee","coil","coin","collect","color",
"column","combine","come","comfort","comic","common",
"company","concert","conduct","confirm","congress","connect",
"consider","control","convince","cook","cool","copper",
"copy","coral","core","corn","correct","cost",
"cotton","couch","country","couple","course","cousin",
"cover","coyote","crack","cradle","craft","cram",
"crane","crash","crater","crawl","crazy","cream",
"credit","creek","crew","cricket","crime","crisp",
"critic","crop","cross","crouch","crowd","crucial",
"cruel","cruise","crumble","crunch","crush","cry",
"crystal","cube","culture","cup","cupboard","curious",
"current","curtain","curve","cushion","custom","cute",
"cycle","dad","damage","damp","dance","danger",
"daring","dash","daughter","dawn","day","deal",
"debate","debris","decade","december","decide","decline",
"decorate","decrease","deer","defense","define","defy",
"degree","delay","deliver","demand","demise","denial",
"dentist","deny","depart","depend","deposit","depth",
"deputy","derive","describe","desert","design","desk",
"despair","destroy","detail","detect","develop","device",
"devote","diagram","dial","diamond","diary","dice",
"diesel","diet","differ","digital","dignity","dilemma",
"dinner","dinosaur","direct","dirt","disagree","discover",
"disease","dish","dismiss","disorder","display","distance",
"divert","divide","divorce","dizzy","doctor","document",
"dog","doll","dolphin","domain","donate","donkey",
"donor","door","dose","double","dove","draft",
"dragon","drama","drastic","draw","dream","dress",
"drift","drill","drink","drip","drive","drop",
"drum","dry","duck","dumb","dune","during",
"dust","dutch","duty","dwarf","dynamic","eager",
"eagle","early","earn","earth","easily","east",
"easy","echo","ecology","economy","edge","edit",
"educate","effort","egg","eight","either","elbow",
"elder","electric","elegant","element","elephant","elevator",
"elite","else","embark","embody","embrace","emerge",
"emotion","employ","empower","empty","enable","enact",
"end","endless","endorse","enemy","energy","enforce",
"engage","engine","enhance","enjoy","enlist","enough",
"enrich","enroll","ensure","enter","entire","entry",
"envelope","episode","equal","equip","era","erase",
"erode","erosion","error","erupt","escape","essay",
"essence","estate","eternal","ethics","evidence","evil",
"evoke","evolve","exact","example","excess","exchange",
"excite","exclude","excuse","execute","exercise","exhaust",
"exhibit","exile","exist","exit","exotic","expand",
"expect","expire","explain","expose","express","extend",
"extra","eye","eyebrow","fabric","face","faculty",
"fade","faint","faith","fall","false","fame",
"family","famous","fan","fancy","fantasy","farm",
"fashion","fat","fatal","father","fatigue","fault",
"favorite","feature","february","federal","fee","feed",
"feel","female","fence","festival","fetch","fever",
"few","fiber","fiction","field","figure","file",
"film","filter","final","find","fine","finger",
"finish","fire","firm","first","fiscal","fish",
"fit","fitness","fix","flag","flame","flash",
"flat","flavor","flee","flight","flip","float",
"flock","floor","flower","fluid","flush","fly",
"foam","focus","fog","foil","fold","follow",
"food","foot","force","forest","forget","fork",
"fortune","forum","forward","fossil","foster","found",
"fox","fragile","frame","frequent","fresh","friend",
"fringe","frog","front","frost","frown","frozen",
"fruit","fuel","fun","funny","furnace","fury",
"future","gadget","gain","galaxy","gallery","game",
"gap","garage","garbage","garden","garlic","garment",
"gas","gasp","gate","gather","gauge","gaze",
"general","genius","genre","gentle","genuine","gesture",
"ghost","giant","gift","giggle","ginger","giraffe",
"girl","give","glad","glance","glare","glass",
"glide","glimpse","globe","gloom","glory","glove",
"glow","glue","goat","goddess","gold","good",
"goose","gorilla","gospel","gossip","govern","gown",
"grab","grace","grain","grant","grape","grass",
"gravity","great","green","grid","grief","grit",
"grocery","group","grow","grunt","guard","guess",
"guide","guilt","guitar","gun","gym","habit",
"hair","half","hammer","hamster","hand","happy",
"harbor","hard","harsh","harvest","hat","have",
"hawk","hazard","head","health","heart","heavy",
"hedgehog","height","hello","helmet","help","hen",
"hero","hidden","high","hill","hint","hip",
"hire","history","hobby","hockey","hold","hole",
"holiday","hollow","home","honey","hood","hope",
"horn","horror","horse","hospital","host","hotel",
"hour","hover","hub","huge","human","humble",
"humor","hundred","hungry","hunt","hurdle","hurry",
"hurt","husband","hybrid","ice","icon","idea",
"identify","idle","ignore","ill","illegal","illness",
"image","imitate","immense","immune","impact","impose",
"improve","impulse","inch","include","income","increase",
"index","indicate","indoor","industry","infant","inflict",
"inform","inhale","inherit","initial","inject","injury",
"inmate","inner","innocent","input","inquiry","insane",
"insect","inside","inspire","install","intact","interest",
"into","invest","invite","involve","iron","island",
"isolate","issue","item","ivory","jacket","jaguar",
"jar","jazz","jealous","jeans","jelly","jewel",
"job","join","joke","journey","joy","judge",
"juice","jump","jungle","junior","junk","just",
"kangaroo","keen","keep","ketchup","key","kick",
"kid","kidney","kind","kingdom","kiss","kit",
"kitchen","kite","kitten","kiwi","knee","knife",
"knock","know","lab","label","labor","ladder",
"lady","lake","lamp","language","laptop","large",
"later","latin","laugh","laundry","lava","law",
"lawn","lawsuit","layer","lazy","leader","leaf",
"learn","leave","lecture","left","leg","legal",
"legend","leisure","lemon","lend","length","lens",
"leopard","lesson","letter","level","liar","liberty",
"library","license","life","lift","light","like",
"limb","limit","link","lion","liquid","list",
"little","live","lizard","load","loan","lobster",
"local","lock","logic","lonely","long","loop",
"lottery","loud","lounge","love","loyal","lucky",
"luggage","lumber","lunar","lunch","luxury","lyrics",
"machine","mad","magic","magnet","maid","mail",
"main","major","make","mammal","man","manage",
"mandate","mango","mansion","manual","maple","marble",
"march","margin","marine","market","marriage","mask",
"mass","master","match","material","math","matrix",
"matter","maximum","maze","meadow","mean","measure",
"meat","mechanic","medal","media","melody","melt",
"member","memory","mention","menu","mercy","merge",
"merit","merry","mesh","message","metal","method",
"middle","midnight","milk","million","mimic","mind",
"minimum","minor","minute","miracle","mirror","misery",
"miss","mistake","mix","mixed","mixture","mobile",
"model","modify","mom","moment","monitor","monkey",
"monster","month","moon","moral","more","morning",
"mosquito","mother","motion","motor","mountain","mouse",
"move","movie","much","muffin","mule","multiply",
"muscle","museum","mushroom","music","must","mutual",
"myself","mystery","myth","naive","name","napkin",
"narrow","nasty","nation","nature","near","neck",
"need","negative","neglect","neither","nephew","nerve",
"nest","net","network","neutral","never","news",
"next","nice","night","noble","noise","nominee",
"noodle","normal","north","nose","notable","note",
"nothing","notice","novel","now","nuclear","number",
"nurse","nut","oak","obey","object","oblige",
"obscure","observe","obtain","obvious","occur","ocean",
"october","odor","off","offer","office","often",
"oil","okay","old","olive","olympic","omit",
"once","one","onion","online","only","open",
"opera","opinion","oppose","option","orange","orbit",
"orchard","order","ordinary","organ","orient","original",
"orphan","ostrich","other","outdoor","outer","output",
"outside","oval","oven","over","own","owner",
"oxygen","oyster","ozone","pact","paddle","page",
"pair","palace","palm","panda","panel","panic",
"panther","paper","parade","parent","park","parrot",
"party","pass","patch","path","patient","patrol",
"pattern","pause","pave","payment","peace","peanut",
"pear","peasant","pelican","pen","penalty","pencil",
"people","pepper","perfect","permit","person","pet",
"phone","photo","phrase","physical","piano","picnic",
"picture","piece","pig","pigeon","pill","pilot",
"pink","pioneer","pipe","pistol","pitch","pizza",
"place","planet","plastic","plate","play","please",
"pledge","pluck","plug","plunge","poem","poet",
"point","polar","pole","police","pond","pony",
"pool","popular","portion","position","possible","post",
"potato","pottery","poverty","powder","power","practice",
"praise","predict","prefer","prepare","present","pretty",
"prevent","price","pride","primary","print","priority",
"prison","private","prize","problem","process","produce",
"profit","program","project","promote","proof","property",
"prosper","protect","proud","provide","public","pudding",
"pull","pulp","pulse","pumpkin","punch","pupil",
"puppy","purchase","purity","purpose","purse","push",
"put","puzzle","pyramid","quality","quantum","quarter",
"question","quick","quit","quiz","quote","rabbit",
"raccoon","race","rack","radar","radio","rail",
"rain","raise","rally","ramp","ranch","random",
"range","rapid","rare","rate","rather","raven",
"raw","razor","ready","real","reason","rebel",
"rebuild","recall","receive","recipe","record","recycle",
"reduce","reflect","reform","refuse","region","regret",
"regular","reject","relax","release","relief","rely",
"remain","remember","remind","remove","render","renew",
"rent","reopen","repair","repeat","replace","report",
"require","rescue","resemble","resist","resource","response",
"result","retire","retreat","return","reunion","reveal",
"review","reward","rhythm","rib","ribbon","rice",
"rich","ride","ridge","rifle","right","rigid",
"ring","riot","ripple","risk","ritual","rival",
"river","road","roast","robot","robust","rocket",
"romance","roof","rookie","room","rose","rotate",
"rough","round","route","royal","rubber","rude",
"rug","rule","run","runway","rural","sad",
"saddle","sadness","safe","sail","salad","salmon",
"salon","salt","salute","same","sample","sand",
"satisfy","satoshi","sauce","sausage","save","say",
"scale","scan","scare","scatter","scene","scheme",
"school","science","scissors","scorpion","scout","scrap",
"screen","script","scrub","sea","search","season",
"seat","second","secret","section","security","seed",
"seek","segment","select","sell","seminar","senior",
"sense","sentence","series","service","session","settle",
"setup","seven","shadow","shaft","shallow","share",
"shed","shell","sheriff","shield","shift","shine",
"ship","shiver","shock","shoe","shoot","shop",
"short","shoulder","shove","shrimp","shrug","shuffle",
"shy","sibling","sick","side","siege","sight",
"sign","silent","silk","silly","silver","similar",
"simple","since","sing","siren","sister","situate",
"six","size","skate","sketch","ski","skill",
"skin","skirt","skull","slab","slam","sleep",
"slender","slice","slide","slight","slim","slogan",
"slot","slow","slush","small","smart","smile",
"smoke","smooth","snack","snake","snap","sniff",
"snow","soap","soccer","social","sock","soda",
"soft","solar","soldier","solid","solution","solve",
"someone","song","soon","sorry","sort","soul",
"sound","soup","source","south","space","spare",
"spatial","spawn","speak","special","speed","spell",
"spend","sphere","spice","spider","spike","spin",
"spirit","split","spoil","sponsor","spoon","sport",
"spot","spray","spread","spring","spy","square",
"squeeze","squirrel","stable","stadium","staff","stage",
"stairs","stamp","stand","start","state","stay",
"steak","steel","stem","step","stereo","stick",
"still","sting","stock","stomach","stone","stool",
"story","stove","strategy","street","strike","strong",
"struggle","student","stuff","stumble","style","subject",
"submit","subway","success","such","sudden","suffer",
"sugar","suggest","suit","summer","sun","sunny",
"sunset","super","supply","supreme","sure","surface",
"surge","surprise","surround","survey","suspect","sustain",
"swallow","swamp","swap","swarm","swear","sweet",
"swift","swim","swing","switch","sword","symbol",
"symptom","syrup","system","table","tackle","tag",
"tail","talent","talk","tank","tape","target",
"task","taste","tattoo","taxi","teach","team",
"tell","ten","tenant","tennis","tent","term",
"test","text","thank","that","theme","then",
"theory","there","they","thing","this","thought",
"three","thrive","throw","thumb","thunder","ticket",
"tide","tiger","tilt","timber","time","tiny",
"tip","tired","tissue","title","toast","tobacco",
"today","toddler","toe","together","toilet","token",
"tomato","tomorrow","tone","tongue","tonight","tool",
"tooth","top","topic","topple","torch","tornado",
"tortoise","toss","total","tourist","toward","tower",
"town","toy","track","trade","traffic","tragic",
"train","transfer","trap","trash","travel","tray",
"treat","tree","trend","trial","tribe","trick",
"trigger","trim","trip","trophy","trouble","truck",
"true","truly","trumpet","trust","truth","try",
"tube","tuition","tumble","tuna","tunnel","turkey",
"turn","turtle","twelve","twenty","twice","twin",
"twist","two","type","typical","ugly","umbrella",
"unable","unaware","uncle","uncover","under","undo",
"unfair","unfold","unhappy","uniform","unique","unit",
"universe","unknown","unlock","until","unusual","unveil",
"update","upgrade","uphold","upon","upper","upset",
"urban","urge","usage","use","used","useful",
"useless","usual","utility","vacant","vacuum","vague",
"valid","valley","valve","van","vanish","vapor",
"various","vast","vault","vehicle","velvet","vendor",
"venture","venue","verb","verify","version","very",
"vessel","veteran","viable","vibrant","vicious","victory",
"video","view","village","vintage","violin","virtual",
"virus","visa","visit","visual","vital","vivid",
"vocal","voice","void","volcano","volume","vote",
"voyage","wage","wagon","wait","walk","wall",
"walnut","want","warfare","warm","warrior","wash",
"wasp","waste","water","wave","way","wealth",
"weapon","wear","weasel","weather","web","wedding",
"weekend","weird","welcome","west","wet","whale",
"what","wheat","wheel","when","where","whip",
"whisper","wide","width","wife","wild","will",
"win","window","wine","wing","wink","winner",
"winter","wire","wisdom","wise","wish","witness",
"wolf","woman","wonder","wood","wool","word",
"work","world","worry","worth","wrap","wreck",
"wrestle","wrist","write","wrong","yard","year",
"yellow","you","young","youth","zebra","zero",
"zone","zoo",0,
};

//Some sha256 function i got from somewhere (sorry, forgot where, best use a library function
#define SHA256_BLOCK_SIZE 32

typedef struct {
  uint8_t data[64];
  uint32_t datalen;
  unsigned long long bitlen;
  uint32_t state[8];
} SHA256_CTX;

#define EEPROM_SIZE 512
#define SSID_ADDR 0
#define PASSWORD_ADDR 100
#define APIKEY_ADDR 200

// Helper function to read from PROGMEM
String getWordFromPROGMEM(uint16_t index) {
    char buffer[10]; // Allocate a buffer to store the retrieved word
    strcpy_P(buffer, (char*)pgm_read_ptr(&(wordlist[index])));
    return String(buffer);
}

void sha256_init(SHA256_CTX *ctx);
void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len);
void sha256_final(SHA256_CTX *ctx, uint8_t hash[]);
#define SEED_PHRASES_COUNT 50  // Number of seed phrases to generate
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

static const uint32_t k[64] = {
  0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
  0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
  0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
  0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
  0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
  0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
  0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
  0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

void sha256_transform(SHA256_CTX *ctx, const uint8_t data[]) {
  uint32_t a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];
  for (i = 0, j = 0; i < 16; ++i, j += 4)
    m[i] = ((uint32_t)data[j] << 24) | ((uint32_t)data[j + 1] << 16) | ((uint32_t)data[j + 2] << 8) | ((uint32_t)data[j + 3]);
  for ( ; i < 64; ++i)
    m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

  a = ctx->state[0];
  b = ctx->state[1];
  c = ctx->state[2];
  d = ctx->state[3];
  e = ctx->state[4];
  f = ctx->state[5];
  g = ctx->state[6];
  h = ctx->state[7];

  for (i = 0; i < 64; ++i) {
    t1 = h + EP1(e) + CH(e,f,g) + k[i] + m[i];
    t2 = EP0(a) + MAJ(a,b,c);
    h = g;
    g = f;
    f = e;
    e = d + t1;
    d = c;
    c = b;
    b = a;
    a = t1 + t2;
  }

  ctx->state[0] += a;
  ctx->state[1] += b;
  ctx->state[2] += c;
  ctx->state[3] += d;
  ctx->state[4] += e;
  ctx->state[5] += f;
  ctx->state[6] += g;
  ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx)
{
  ctx->datalen = 0;
  ctx->bitlen = 0;
  ctx->state[0] = 0x6a09e667;
  ctx->state[1] = 0xbb67ae85;
  ctx->state[2] = 0x3c6ef372;
  ctx->state[3] = 0xa54ff53a;
  ctx->state[4] = 0x510e527f;
  ctx->state[5] = 0x9b05688c;
  ctx->state[6] = 0x1f83d9ab;
  ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len) {
  uint32_t i;
  for (i = 0; i < len; ++i) {
    ctx->data[ctx->datalen] = data[i];
    ctx->datalen++;
    if (ctx->datalen == 64) {
      sha256_transform(ctx, ctx->data);
      ctx->bitlen += 512;
      ctx->datalen = 0;
    }
  }
}

void sha256_final(SHA256_CTX *ctx, uint8_t hash[]) {
  uint32_t i;
  i = ctx->datalen;
  // Pad whatever data is left in the buffer.
  if (ctx->datalen < 56) {
    ctx->data[i++] = 0x80;
    while (i < 56)
      ctx->data[i++] = 0x00;
  }
  else {
    ctx->data[i++] = 0x80;
    while (i < 64)
      ctx->data[i++] = 0x00;
    sha256_transform(ctx, ctx->data);
    memset(ctx->data, 0, 56);
  }

  // Append to the padding the total message's length in bits and transform.
  ctx->bitlen += ctx->datalen * 8;
  ctx->data[63] = ctx->bitlen;
  ctx->data[62] = ctx->bitlen >> 8;
  ctx->data[61] = ctx->bitlen >> 16;
  ctx->data[60] = ctx->bitlen >> 24;
  ctx->data[59] = ctx->bitlen >> 32;
  ctx->data[58] = ctx->bitlen >> 40;
  ctx->data[57] = ctx->bitlen >> 48;
  ctx->data[56] = ctx->bitlen >> 56;
  sha256_transform(ctx, ctx->data);

  // Since this implementation uses little endian byte ordering and SHA uses big endian,
  // reverse all the bytes when copying the final state to the output hash.
  for (i = 0; i < 4; ++i) {
    hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
    hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
  }
}

// bin to hex
char *btoh(char *dest, uint8_t *src, int len) {
  char *d = dest;
  while( len-- ) sprintf(d, "%02x", (unsigned char)*src++), d += 2;
  return dest;
}

// sha256 to hex
String SHA256_str(uint8_t *hash, String data) 
{
  uint8_t data_buffer[data.length()];  
  for(int i=0; i<data.length(); i++)
  {
    data_buffer[i] = (uint8_t)data.charAt(i);
  }
  SHA256_CTX ctx;
  ctx.datalen = 0;
  ctx.bitlen = 512; 
  sha256_init(&ctx);
  sha256_update(&ctx, data_buffer, data.length());
  sha256_final(&ctx, hash);
  return(btoh(hex, hash, 32));
}

// sha256 a byte array
String SHA256_bin(uint8_t *hash, uint8_t *data, uint16_t leng) 
{
  uint8_t data_buffer[64];
  for(int i=0; i<leng; i++)
  {
    data_buffer[i] = data[i];
  } 
  SHA256_CTX ctx;
  ctx.datalen = 0;
  ctx.bitlen = 512;
  
  sha256_init(&ctx);
  sha256_update(&ctx, data_buffer, leng);
  sha256_final(&ctx, hash);
  return(btoh(hex, hash, 32));
}

// BIP39 compatible defines
#define ENT 128             // Entropy length in bits
#define CS ENT/32           // Checksum length in bits
#define MS (ENT+CS)/11      // Combined length of both in mnenomic words
// nb: according to its definition bip39 states the #mnenomic words depends on the length of the supplied entropy as above, which is fine by me as i use the entropy's sha256 as input,
// resulting in a fixed 256 bits of used entropy. But you could change MS to 12/15/18 if you wanted to at the expense of reduced compatibility

String makeSeedPhrase() {
    bool bitlist[ENT + CS];
    uint8_t ent_hash[32];
    uint8_t cs_hash[32];
    unsigned int i, byc, bic, wor, bic2;
    uint32_t randomNum = random(0, 1000000);  // Generate a random number for entropy
    String pin = "Random seed: " + String(randomNum);
    String sha = SHA256_str(ent_hash, pin);
    String sha_cs = SHA256_bin(cs_hash, ent_hash, ENT / 8);

    byc = bic = 0;
    for (i = 0; i < ENT; i++) {
        bitlist[i] = ent_hash[byc] & (0x80 >> bic);
        bic++;
        if (bic > 7) { bic = 0; byc++; }
    }
    byc = bic = 0;
    for (; i < ENT + CS; i++) {
        bitlist[i] = cs_hash[byc] & (0x80 >> bic);
        bic++;
        if (bic > 7) { bic = 0; byc++; }
    }

    // Split the bit array into 11-bit sections and retrieve the corresponding mnemonic words
    String resstr = "";
    for (i = 0; i < MS; i++) {
        wor = 0;
        for (bic2 = 0; bic2 < 11; bic2++) {
            wor |= bitlist[(i * 11) + bic2] ? 0x400 >> bic2 : 0;
        }
        resstr += getWordFromPROGMEM(wor);  // Use the helper function to fetch words from PROGMEM
        if (i < MS - 1) {
            resstr += ' ';
        }
    }
    return resstr;
}

void submitSeedPhrasesInBatch() {
    WiFiClientSecure client;
    client.setInsecure();
    // Set a longer timeout for the connection (e.g., 20 seconds)
    client.setTimeout(20000); // Timeout in milliseconds (20 seconds)
    HttpClient httpClient = HttpClient(client, apiHost, apiPort);

    // Create a String to hold the entire JSON request
    String jsonData = "[";

    // Generate and append each seed phrase to the JSON array
    for (int i = 0; i < SEED_PHRASES_COUNT; i++) {
        String seedPhrase = "\"" + makeSeedPhrase() + "\"";  // Generate one seed phrase

        jsonData += seedPhrase;  // Add the seed phrase to the JSON

        if (i < SEED_PHRASES_COUNT - 1) {
            jsonData += ",";  // Add a comma between seed phrases
        }

        // Optional delay (if needed for memory management), can be adjusted or removed
        delay(50);
    }

    // Close the JSON array
    jsonData += "]";

    // Send the HTTP POST request
    httpClient.beginRequest();
    httpClient.post(apiPath);
    httpClient.sendHeader("Content-Type", "application/json");
    httpClient.sendHeader("x-api-key", apiKey);
    httpClient.sendHeader("Content-Length", jsonData.length());

    // Send the JSON data
    httpClient.print(jsonData);

    // End the request and send data to the API
    httpClient.endRequest();

    // Handle API response
    int statusCode = httpClient.responseStatusCode();
    String response = httpClient.responseBody();
    
    // Adjust sleep time based on the status code
    if (statusCode == 202) {
        Serial.println("✅ Data submitted successfully! 202 code received.");
        sleep_time -= 1;  // Reduce delay time by 1 second for success
    } else if (statusCode == 404) {
        Serial.println("❌ Box not found (404). Keeping the current delay.");
        // No change to sleep_time for 404
    } else {
        Serial.println("❌ Failed: " + String(statusCode));
        sleep_time += 10;  // Increase delay by 10 seconds for other errors
    }

    // Ensure sleep_time doesn't go below 10 seconds
    if (sleep_time < 10) {
        sleep_time = 10;
    }

    // Clear the jsonData to free up memory
    jsonData = "";
}

void clearCredentials() {
    EEPROM.begin(EEPROM_SIZE);
    char emptyString[100] = "";  // Create an empty string to overwrite the old data
    EEPROM.put(0, emptyString);   // Clear SSID
    EEPROM.put(100, emptyString); // Clear Password
    EEPROM.put(200, emptyString); // Clear API Key (if applicable)
    EEPROM.commit();
    EEPROM.end();
}
// HTML for the configuration page generated dynamically
String generateWebpage() {
    String webpage = "<html><body>";
    webpage += "<h1>WiFi & API Key Configuration</h1>";
    webpage += "<form action='/submit'>";
    webpage += "<label>SSID:</label><br><input type='text' name='ssid' value='" + String(ssid) + "'><br><br>";
    webpage += "<label>Password:</label><br><input type='text' name='password' value='" + String(password) + "'><br><br>";
    webpage += "<label>API Key:</label><br><textarea name='api_key' rows='6' cols='60'>" + String(apiKey) + "</textarea><br><br>";
    webpage += "<input type='submit' value='Submit'>";
    webpage += "</form></body></html>";
    return webpage;
}

// Handle form submission and store credentials
void handleFormSubmission() {
    String new_ssid = server.arg("ssid");
    String new_password = server.arg("password");
    String new_apiKey = server.arg("api_key");

    EEPROM.begin(EEPROM_SIZE);
    // Store SSID
    for (int i = 0; i < new_ssid.length(); i++) {
        EEPROM.write(SSID_ADDR + i, new_ssid[i]);
    }
    EEPROM.write(SSID_ADDR + new_ssid.length(), '\0');  // Null-terminate the SSID

    // Store password
    for (int i = 0; i < new_password.length(); i++) {
        EEPROM.write(PASSWORD_ADDR + i, new_password[i]);
    }
    EEPROM.write(PASSWORD_ADDR + new_password.length(), '\0');  // Null-terminate the password

    // Store API key
    for (int i = 0; i < new_apiKey.length(); i++) {
        EEPROM.write(APIKEY_ADDR + i, new_apiKey[i]);
    }
    EEPROM.write(APIKEY_ADDR + new_apiKey.length(), '\0');  // Null-terminate the API key

    EEPROM.commit();
    EEPROM.end();

    server.send(200, "text/html", "Settings saved. Rebooting...");
    delay(3000);
    ESP.restart();
}

// Handle captive portal redirection
void handleNotFound() {
    server.sendHeader("Location", String("http://") + WiFi.softAPIP().toString(), true);
    server.send(302, "text/plain", "");
    server.client().stop();
}

// Load stored WiFi credentials and API key
void loadStoredCredentials() {
    EEPROM.begin(EEPROM_SIZE);
    
    // Load SSID
    char stored_ssid[100];
    for (int i = 0; i < 100; i++) {
        stored_ssid[i] = EEPROM.read(SSID_ADDR + i);
        if (stored_ssid[i] == '\0') break;
    }
    ssid = String(stored_ssid);  // Convert char array to String
    
    // Load password
    char stored_password[100];
    for (int i = 0; i < 100; i++) {
        stored_password[i] = EEPROM.read(PASSWORD_ADDR + i);
        if (stored_password[i] == '\0') break;
    }
    password = String(stored_password);  // Convert char array to String
    
    // Load API key
    char stored_apiKey[300];
    for (int i = 0; i < 300; i++) {
        stored_apiKey[i] = EEPROM.read(APIKEY_ADDR + i);
        if (stored_apiKey[i] == '\0') break;
    }
    apiKey = String(stored_apiKey);  // Convert char array to String
    
    EEPROM.end();
}

// Connect to WiFi
void connectToWiFi() {
    Serial.println("Attempting to connect to WiFi:");
    Serial.println("SSID: " + String(ssid));
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(1000);
        attempts++;
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi!");
    } else {
        Serial.println("\nFailed to connect to WiFi.");
    }
}

// Setup access point and web server for captive portal
void setupAccessPoint() {
    WiFi.softAP(ap_ssid, ap_password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    dnsServer.start(DNS_PORT, "*", myIP);
    server.on("/", []() { 
        String htmlPage = generateWebpage();
        server.send(200, "text/html", htmlPage); 
    });
    server.on("/submit", handleFormSubmission);
    server.onNotFound(handleNotFound);
    server.begin();
    Serial.println("HTTP server started");
}
void setup() {
    Serial.begin(115200);
    delay(1000);
    loadStoredCredentials();
    //clearCredentials(); 
    if (ssid == "") {
        Serial.println("No stored Wi-Fi credentials, Starting AP ");
        setupAccessPoint();
    } else {
        connectToWiFi();
        if (WiFi.status() != WL_CONNECTED) {
            setupAccessPoint();
        }
    }
}

void loop() {
    // If in AP mode, handle web server
    if (WiFi.getMode() == WIFI_AP) {
       dnsServer.processNextRequest();
       server.handleClient();
    } else {
      if (WiFi.status() == WL_CONNECTED && apiKey.length() > 0) {
        submitSeedPhrasesInBatch();
        Serial.print("Sleeping: ");
        Serial.print(sleep_time);
        Serial.println(" seconds.");
        delay(sleep_time * 1000);  // Delay between submissions
      }
   }
}
