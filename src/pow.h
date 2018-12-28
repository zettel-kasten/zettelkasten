#ifndef POW_H
#define POW_H

#include "uint256.h"
#include "sph_keccak.h"

template<typename T1>
inline uint256 HashSDK(const T1 pbegin, const T1 pend)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];


	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	return hash[1].trim256();
}


template<typename T1>
inline uint256 HashSDKPGAB_EVEN(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	hash[1] =~hash[1];

	temp_hash = hash[1];
	hash[1] = temp_hash>>B;
	temp_hash <<=(512-B);
	hash[1] += temp_hash;

	return hash[1].trim256();
}


template<typename T1>
inline uint256 HashSDKPGAB_ODD(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	hash[0] =~hash[0];

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	temp_hash = hash[1];
	hash[1] = temp_hash>>B;
	temp_hash <<=(512-B);
	hash[1] += temp_hash;

	return hash[1].trim256();
}


static unsigned char SDKPGABSPC_sinetable[] =
{
	0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x05, 0x07,
	0x09, 0x0C, 0x0E, 0x11, 0x14, 0x17, 0x1A, 0x1E,
	0x21, 0x24, 0x27, 0x2A, 0x2D, 0x30, 0x33, 0x36,
	0x38, 0x3A, 0x3C, 0x3D, 0x3F, 0x3F, 0x40, 0x40,
	0x40, 0x40, 0x3F, 0x3F, 0x3D, 0x3C, 0x3A, 0x38,
	0x36, 0x33, 0x30, 0x2D, 0x2A, 0x27, 0x24, 0x21,
	0x1E, 0x1A, 0x17, 0x14, 0x11, 0x0E, 0x0C, 0x09,
	0x07, 0x05, 0x03, 0x02, 0x01, 0x00, 0x00, 0x00,
};

template<typename T1>
inline uint256 HashSDKPGABSPC_EVEN(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B, uint32_t sinetable_pos)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), SDKPGABSPC_sinetable[sinetable_pos] * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64-SDKPGABSPC_sinetable[sinetable_pos]);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	hash[1] =~hash[1];

	temp_hash = hash[1];
	hash[1] = temp_hash>>B;
	temp_hash <<=(512-B);
	hash[1] += temp_hash;

	return hash[1].trim256();
}


template<typename T1>
inline uint256 HashSDKPGABSPC_ODD(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B, uint32_t sinetable_pos)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), SDKPGABSPC_sinetable[sinetable_pos] * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	hash[0] =~hash[0];

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64-SDKPGABSPC_sinetable[sinetable_pos]);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	temp_hash = hash[1];
	hash[1] = temp_hash>>B;
	temp_hash <<=(512-B);
	hash[1] += temp_hash;

	return hash[1].trim256();
}


static std::string noun_list[1222] = {
	"ability", "accident", "account", "acid", "action", "actor", "actress", "addict", "address", "adult",
	"advice", "affair", "age", "agent", "air", "airport", "aisle", "alarm", "album", "alcohol",
	"alien", "alley", "alpha", "amateur", "amount", "analyst", "anchor", "anger", "angle", "animal",
	"ankle", "answer", "antenna", "anxiety", "apology", "apple", "april", "arch", "area", "arena",
	"armor", "army", "arrow", "art", "artefact", "artist", "artwork", "aspect", "asset", "asthma",
	"athlete", "atom", "attitude", "auction", "audit", "august", "aunt", "author", "auto", "autumn",
	"avocado", "axis", "baby", "bachelor", "bacon", "badge", "bag", "balance", "balcony", "ball",
	"bamboo", "banana", "banner", "bar", "bargain", "barrel", "base", "basket", "battle", "beach",
	"bean", "beauty", "beef", "belt", "bench", "benefit", "bicycle", "bike", "biology", "bird",
	"birth", "blade", "blanket", "blast", "blood", "blouse", "blur", "blush", "board", "boat",
	"body", "bomb", "bone", "bonus", "book", "boost", "border", "boss", "bounce", "box",
	"boy", "bracket", "brain", "brand", "brass", "bread", "breeze", "brick", "bridge", "broccoli",
	"broom", "brother", "brush", "bubble", "buddy", "budget", "buffalo", "bulb", "bullet", "bundle",
	"bunker", "burden", "burger", "bus", "business", "butter", "buyer", "buzz", "cabbage", "cabin",
	"cable", "cactus", "cage", "cake", "camera", "camp", "canal", "candy", "cannon", "canoe",
	"canvas", "canyon", "capital", "captain", "car", "carbon", "card", "cargo", "carpet", "cart",
	"case", "cash", "casino", "castle", "cat", "catalog", "category", "cattle", "caution", "cave",
	"ceiling", "celery", "cement", "census", "century", "cereal", "chair", "chalk", "champion", "chaos",
	"chapter", "cheese", "chef", "cherry", "chest", "chicken", "chief", "child", "chimney", "choice",
	"chunk", "cigar", "cinnamon", "circle", "citizen", "city", "claw", "clay", "clerk", "client",
	"cliff", "clinic", "clip", "clock", "cloth", "cloud", "clown", "club", "clump", "cluster",
	"coach", "coast", "coconut", "code", "coffee", "coin", "color", "column", "comfort", "comic",
	"company", "concert", "congress", "copper", "coral", "core", "corn", "cotton", "couch", "country",
	"couple", "course", "cousin", "coyote", "cradle", "craft", "crane", "crater", "cream", "credit",
	"creek", "crew", "cricket", "crime", "crop", "cross", "crowd", "crystal", "cube", "culture",
	"cup", "cupboard", "curtain", "curve", "cushion", "custom", "cycle", "dad", "danger", "dash",
	"daughter", "dawn", "day", "deal", "debris", "decade", "december", "deer", "defense", "degree",
	"delay", "demise", "denial", "dentist", "deposit", "depth", "deputy", "desert", "desk", "despair",
	"device", "diagram", "diamond", "diary", "dice", "diesel", "diet", "dignity", "dilemma", "dinner",
	"dinosaur", "dirt", "disease", "dish", "disorder", "display", "distance", "doctor", "document", "dog",
	"doll", "dolphin", "domain", "donkey", "donor", "door", "dose", "dove", "draft", "dragon",
	"drama", "dream", "dress", "drum", "duck", "dune", "dust", "duty", "dwarf", "eagle",
	"earth", "echo", "ecology", "economy", "edge", "effort", "egg", "elbow", "elder", "element",
	"elephant", "elevator", "elite", "emotion", "enemy", "energy", "engine", "entry", "envelope", "episode",
	"era", "erosion", "error", "essay", "essence", "estate", "ethics", "evidence", "example", "excess",
	"exchange", "exercise", "exile", "exit", "eye", "eyebrow", "fabric", "face", "faculty", "faith",
	"fame", "family", "fan", "fantasy", "farm", "fashion", "father", "fatigue", "fault", "feature",
	"february", "fee", "fence", "festival", "fever", "fiber", "fiction", "field", "figure", "file",
	"film", "filter", "finger", "fire", "firm", "fish", "fitness", "flag", "flame", "flash",
	"flavor", "flight", "floor", "flower", "fluid", "foam", "focus", "fog", "foil", "food",
	"foot", "force", "forest", "fork", "fortune", "forum", "fossil", "fox", "frame", "friend",
	"frog", "frost", "fruit", "fuel", "fun", "furnace", "fury", "future", "gadget", "galaxy",
	"gallery", "game", "gap", "garage", "garbage", "garden", "garlic", "garment", "gas", "gasp",
	"gate", "gauge", "gaze", "genre", "gesture", "ghost", "gift", "giraffe", "girl", "glass",
	"globe", "gloom", "glory", "glove", "glue", "goat", "goddess", "gold", "goose", "gorilla",
	"gospel", "gossip", "gown", "grace", "grain", "grape", "grass", "gravity", "grid", "grief",
	"grit", "grocery", "group", "guard", "guess", "guide", "guilt", "guitar", "gun", "gym",
	"habit", "hair", "half", "hammer", "hamster", "hand", "harbor", "harvest", "hat", "hawk",
	"hazard", "head", "health", "heart", "hedgehog", "height", "hello", "helmet", "help", "hen",
	"hero", "hill", "hint", "hip", "history", "hobby", "hockey", "hole", "holiday", "home",
	"honey", "hood", "hope", "horn", "horror", "horse", "hospital", "host", "hotel", "hour",
	"hover", "hub", "human", "humor", "hurdle", "husband", "ice", "icon", "idea", "illness",
	"image", "impulse", "inch", "income", "index", "industry", "infant", "injury", "inmate", "input",
	"inquiry", "insect", "interest", "iron", "island", "item", "ivory", "jacket", "jaguar", "jar",
	"jazz", "jeans", "jelly", "jewel", "job", "joke", "journey", "joy", "judge", "juice",
	"jungle", "junior", "junk", "kangaroo", "ketchup", "key", "kid", "kidney", "kingdom", "kiss",
	"kit", "kitchen", "kite", "kitten", "kiwi", "knee", "knife", "lab", "label", "labor",
	"ladder", "lady", "lake", "lamp", "language", "laptop", "laundry", "lava", "law", "lawn",
	"lawsuit", "layer", "leader", "leaf", "lecture", "leg", "legend", "leisure", "lemon", "length",
	"lens", "leopard", "lesson", "letter", "level", "liar", "liberty", "library", "license", "life",
	"lift", "limb", "limit", "lion", "liquid", "list", "lizard", "loan", "lobster", "lock",
	"logic", "loop", "lottery", "lounge", "love", "luggage", "lumber", "lunch", "luxury", "lyrics",
	"machine", "magic", "magnet", "maid", "mail", "mammal", "man", "mandate", "mango", "mansion",
	"manual", "maple", "marble", "march", "margin", "marine", "market", "marriage", "mask", "mass",
	"master", "match", "material", "math", "matrix", "matter", "maze", "meadow", "measure", "meat",
	"mechanic", "medal", "media", "melody", "member", "memory", "menu", "mercy", "merit", "mesh",
	"message", "metal", "method", "midnight", "milk", "mind", "minute", "miracle", "mirror", "misery",
	"mistake", "mix", "mixture", "model", "mom", "moment", "monitor", "monkey", "monster", "month",
	"moon", "morning", "mosquito", "mother", "motion", "motor", "mountain", "mouse", "movie", "muffin",
	"mule", "muscle", "museum", "mushroom", "music", "myself", "mystery", "myth", "name", "napkin",
	"nation", "nature", "neck", "nephew", "nerve", "nest", "net", "network", "news", "night",
	"noise", "nominee", "noodle", "north", "nose", "note", "nothing", "notice", "novel", "number",
	"nurse", "nut", "oak", "object", "ocean", "october", "odor", "office", "oil", "olive",
	"onion", "opera", "opinion", "option", "orange", "orbit", "orchard", "organ", "orient", "orphan",
	"ostrich", "output", "oven", "owner", "oxygen", "oyster", "ozone", "pact", "paddle", "page",
	"pair", "palace", "palm", "panda", "panel", "panic", "panther", "paper", "parade", "parent",
	"park", "parrot", "party", "patch", "path", "patient", "patrol", "pattern", "pause", "pave",
	"payment", "peace", "peanut", "pear", "peasant", "pelican", "pen", "penalty", "pencil", "people",
	"pepper", "permit", "person", "pet", "phone", "photo", "phrase", "piano", "picnic", "picture",
	"piece", "pig", "pigeon", "pill", "pilot", "pioneer", "pipe", "pistol", "pizza", "place",
	"planet", "plate", "poem", "poet", "point", "pole", "police", "pond", "pony", "pool",
	"portion", "position", "potato", "pottery", "poverty", "powder", "power", "practice", "price", "pride",
	"prison", "prize", "problem", "process", "profit", "program", "project", "proof", "property", "pudding",
	"pulp", "pulse", "pumpkin", "pupil", "puppy", "purchase", "purity", "purpose", "purse", "puzzle",
	"pyramid", "quality", "quarter", "question", "quiz", "quote", "rabbit", "raccoon", "race", "rack",
	"radar", "radio", "rail", "rain", "rally", "ramp", "ranch", "range", "raven", "razor",
	"reason", "rebel", "recall", "recipe", "record", "reform", "region", "release", "relief", "rent",
	"report", "resource", "response", "result", "retreat", "reunion", "review", "reward", "rhythm", "rib",
	"ribbon", "rice", "ride", "ridge", "rifle", "ring", "riot", "ripple", "risk", "ritual",
	"rival", "river", "road", "roast", "robot", "rocket", "romance", "roof", "rookie", "room",
	"rose", "rubber", "rug", "rule", "runway", "sadness", "salad", "salmon", "salon", "salt",
	"salute", "sample", "sand", "satoshi", "sauce", "sausage", "scale", "scene", "scheme", "school",
	"science", "scissors", "scorpion", "scout", "scrap", "screen", "script", "scrub", "sea", "season",
	"seat", "secret", "section", "security", "seed", "segment", "seminar", "senior", "sense", "sentence",
	"series", "service", "session", "setup", "shadow", "shaft", "shell", "sheriff", "shield", "shift",
	"ship", "shiver", "shock", "shoe", "shop", "shoulder", "shrimp", "sibling", "side", "siege",
	"sight", "sign", "silk", "silver", "siren", "sister", "size", "sketch", "ski", "skill",
	"skin", "skirt", "skull", "slab", "slice", "slogan", "slot", "slush", "smile", "smoke",
	"snack", "snake", "snow", "soap", "soccer", "sock", "soda", "soldier", "solution", "someone",
	"song", "soul", "sound", "soup", "source", "space", "speed", "sphere", "spice", "spider",
	"spike", "spin", "spirit", "sponsor", "spoon", "sport", "spot", "spray", "spring", "spy",
	"square", "squirrel", "stadium", "staff", "stage", "stairs", "stamp", "stand", "start", "state",
	"steak", "steel", "stem", "step", "stick", "stomach", "stone", "stool", "story", "stove",
	"strategy", "street", "strike", "struggle", "student", "stuff", "style", "subject", "subway", "success",
	"sugar", "suit", "summer", "sun", "sunset", "supply", "surface", "surge", "surprise", "survey",
	"swamp", "swarm", "sword", "symbol", "symptom", "syrup", "system", "table", "tag", "tail",
	"talent", "tank", "tape", "target", "task", "tattoo", "taxi", "team", "tenant", "tennis",
	"tent", "term", "test", "text", "theme", "theory", "they", "thing", "this", "thought",
	"thumb", "thunder", "ticket", "tide", "tiger", "tilt", "timber", "time", "tip", "tissue",
	"title", "toast", "tobacco", "toddler", "toe", "toilet", "tomato", "tone", "tongue", "tool",
	"tooth", "topic", "torch", "tornado", "tortoise", "tourist", "tower", "town", "toy", "traffic",
	"train", "trap", "trash", "tray", "treat", "tree", "trend", "trial", "tribe", "trick",
	"trigger", "trip", "trophy", "trouble", "truck", "trumpet", "trust", "truth", "tube", "tuition",
	"tuna", "tunnel", "turkey", "turtle", "twin", "twist", "type", "umbrella", "uncle", "uniform",
	"unit", "universe", "usage", "utility", "vacuum", "valley", "valve", "van", "vapor", "vault",
	"vehicle", "vendor", "venture", "venue", "verb", "version", "vessel", "veteran", "victory", "video",
	"view", "village", "violin", "virus", "visa", "visit", "voice", "volcano", "volume", "vote",
	"voyage", "wage", "wagon", "wall", "walnut", "warfare", "warrior", "wasp", "waste", "water",
	"wave", "way", "wealth", "weapon", "weasel", "weather", "web", "wedding", "weekend", "whale",
	"wheat", "wheel", "whip", "whisper", "width", "wife", "window", "wine", "wing", "wink",
	"winner", "winter", "wire", "wisdom", "wish", "witness", "wolf", "woman", "wood", "wool",
	"word", "work", "world", "wrap", "wrist", "yard", "year", "you", "youth", "zebra",
	"zone", "zoo",
};

static std::string verb_list[433] = {
	"abandon", "absorb", "abuse", "access", "accuse", "achieve", "acquire", "act", "adapt", "add",
	"adjust", "admit", "advance", "afford", "agree", "aim", "alert", "allow", "alter", "announce",
	"appear", "approve", "argue", "arm", "arrange", "arrest", "arrive", "ask", "assist", "assume",
	"assault", "attack", "attend", "attract", "avoid", "become", "begin", "behave", "believe", "betray",
	"bid", "bind", "blame", "bless", "blossom", "boil", "borrow", "bring", "build", "call",
	"can", "cancel", "carry", "catch", "cause", "change", "charge", "chase", "chat", "check",
	"choose", "chuckle", "churn", "claim", "clap", "clarify", "click", "climb", "clog", "close",
	"clutch", "coil", "collect", "combine", "come", "conduct", "confirm", "connect", "consider", "control",
	"convince", "cook", "copy", "cost", "cover", "crack", "cram", "crash", "crawl", "critic",
	"crouch", "cruise", "crumble", "crunch", "crush", "cry", "damage", "damp", "dance", "debate",
	"decide", "decline", "decorate", "decrease", "define", "defy", "deliver", "demand", "deny", "depart",
	"depend", "derive", "describe", "design", "destroy", "detail", "detect", "develop", "devote", "dial",
	"differ", "disagree", "discover", "dismiss", "divert", "divide", "divorce", "donate", "draw", "drift",
	"drill", "drink", "drip", "drive", "drop", "earn", "edit", "educate", "embark", "embody",
	"embrace", "emerge", "employ", "empower", "enable", "enact", "end", "endorse", "enforce", "engage",
	"enhance", "enjoy", "enlist", "enrich", "enroll", "ensure", "enter", "equip", "erase", "erode",
	"erupt", "escape", "evoke", "evolve", "excite", "exclude", "excuse", "execute", "exhaust", "exhibit",
	"exist", "expand", "expect", "expire", "explain", "expose", "express", "extend", "fade", "faint",
	"fall", "feed", "feel", "fetch", "find", "finish", "fit", "fix", "flee", "flip",
	"float", "flock", "flush", "fly", "fold", "follow", "forget", "forward", "foster", "frown",
	"gain", "gather", "giggle", "give", "glance", "glare", "glide", "glimpse", "glow", "govern",
	"grab", "grant", "grow", "grunt", "have", "hire", "hold", "hunt", "hurry", "hurt",
	"identify", "ignore", "imitate", "impact", "impose", "improve", "include", "increase", "indicate", "inflict",
	"inform", "inhale", "inherit", "inject", "inspire", "install", "invest", "invite", "involve", "isolate",
	"issue", "join", "jump", "keep", "kick", "knock", "know", "laugh", "learn", "leave",
	"lend", "like", "link", "live", "load", "make", "manage", "melt", "mention", "merge",
	"mimic", "miss", "modify", "move", "multiply", "must", "need", "neglect", "obey", "oblige",
	"observe", "obtain", "occur", "offer", "omit", "open", "oppose", "order", "pass", "pitch",
	"play", "please", "pledge", "pluck", "plug", "plunge", "post", "praise", "predict", "prefer",
	"prepare", "present", "prevent", "print", "produce", "promote", "prosper", "protect", "provide", "pull",
	"punch", "push", "put", "quit", "raise", "rate", "rebuild", "receive", "recycle", "reduce",
	"reflect", "refuse", "regret", "reject", "relax", "rely", "remain", "remember", "remind", "remove",
	"render", "renew", "reopen", "repair", "repeat", "replace", "require", "rescue", "resemble", "resist",
	"retire", "return", "reveal", "rotate", "route", "run", "saddle", "sail", "satisfy", "save",
	"say", "scan", "scare", "scatter", "search", "seek", "select", "sell", "settle", "share",
	"shed", "shine", "shoot", "shove", "shrug", "shuffle", "sing", "situate", "skate", "slam",
	"sleep", "slide", "snap", "sniff", "solve", "sort", "spawn", "speak", "spell", "spend",
	"split", "spoil", "spread", "squeeze", "stay", "sting", "stumble", "submit", "suffer", "suggest",
	"surround", "suspect", "sustain", "swallow", "swap", "swear", "swim", "swing", "switch", "tackle",
	"talk", "taste", "teach", "tell", "thank", "thrive", "throw", "topple", "toss", "track",
	"trade", "transfer", "travel", "trim", "try", "tumble", "turn", "uncover", "undo", "unfold",
	"unlock", "unveil", "update", "upgrade", "uphold", "upset", "use", "urge", "vanish", "verify",
	"wait", "walk", "want", "wash", "wear", "welcome", "will", "win", "wonder", "worry",
	"wreck", "wrestle", "write",
};

static std::string adjective_list[323] = {
	"able", "absent", "abstract", "absurd", "acoustic", "actual", "aerobic", "afraid", "all", "alone",
	"amazing", "amused", "ancient", "angry", "annual", "another", "antique", "any", "arctic", "armed",
	"average", "awake", "aware", "awesome", "awful", "awkward", "basic", "best", "better", "bitter",
	"black", "bleak", "blind", "blue", "boring", "bottom", "brave", "brief", "bright", "brisk",
	"broken", "bronze", "brown", "bulk", "burst", "busy", "calm", "capable", "casual", "caught",
	"certain", "cheap", "chronic", "civil", "clean", "clever", "common", "cool", "correct", "crazy",
	"crisp", "crucial", "cruel", "curious", "current", "cute", "daring", "digital", "direct", "dizzy",
	"double", "drastic", "dry", "dumb", "dutch", "dynamic", "eager", "early", "east", "easy",
	"eight", "electric", "elegant", "empty", "endless", "enough", "entire", "equal", "eternal", "evil",
	"exact", "exotic", "extra", "false", "famous", "fancy", "fat", "fatal", "favorite", "federal",
	"female", "few", "final", "fine", "fiscal", "flat", "found", "fragile", "frequent", "fresh",
	"fringe", "front", "frozen", "funny", "general", "genius", "gentle", "genuine", "giant", "ginger",
	"glad", "good", "great", "green", "happy", "hard", "harsh", "heavy", "hidden", "high",
	"hollow", "huge", "humble", "hundred", "hungry", "hybrid", "idle", "ill", "illegal", "immense",
	"immune", "indoor", "initial", "inner", "innocent", "insane", "intact", "jealous", "keen", "kind",
	"large", "latin", "lazy", "legal", "light", "little", "local", "lonely", "long", "loud",
	"loyal", "lucky", "lunar", "mad", "main", "major", "maximum", "mean", "merry", "middle",
	"million", "minimum", "minor", "mixed", "mobile", "moral", "mutual", "naive", "narrow", "nasty",
	"negative", "neutral", "nice", "noble", "normal", "notable", "nuclear", "obscure", "obvious", "okay",
	"old", "olympic", "one", "ordinary", "original", "other", "outdoor", "outer", "oval", "own",
	"perfect", "physical", "pink", "plastic", "polar", "popular", "possible", "pretty", "primary", "priority",
	"private", "proud", "public", "quantum", "quick", "random", "rapid", "rare", "raw", "ready",
	"real", "regular", "rich", "rigid", "robust", "rough", "round", "royal", "rude", "rural",
	"sad", "safe", "same", "seven", "shallow", "short", "shy", "sick", "silent", "silly",
	"similar", "simple", "six", "slender", "slight", "slim", "slow", "small", "smart", "smooth",
	"social", "soft", "solar", "solid", "sorry", "south", "spare", "spatial", "special", "stable",
	"stereo", "stock", "strong", "sudden", "sunny", "super", "supreme", "sweet", "swift", "ten",
	"three", "tiny", "tired", "token", "top", "total", "tragic", "true", "twelve", "twenty",
	"two", "typical", "ugly", "unable", "unaware", "unfair", "unhappy", "unique", "unknown", "unusual",
	"upper", "urban", "used", "useful", "useless", "usual", "vacant", "vague", "valid", "various",
	"vast", "velvet", "viable", "vibrant", "vicious", "vintage", "virtual", "visual", "vital", "vivid",
	"vocal", "void", "warm", "weird", "west", "wet", "wide", "wild", "wise", "wrong",
	"yellow", "young", "zero",
};

static std::string adverb_list[48] = {
	"again", "ahead", "almost", "already", "also", "always", "apart", "around", "away", "barely",
	"easily", "else", "first", "inside", "just", "later", "left", "more", "much", "never",
	"next", "now", "off", "often", "once", "online", "only", "outside", "rather", "right",
	"second", "soon", "still", "such", "sure", "then", "there", "today", "together", "tomorrow",
	"tonight", "truly", "twice", "upon", "very", "what", "when", "where",
};

static std::string preposition_list[15] = {
	"about", "above", "across", "among", "behind", "below", "between", "beyond", "during", "into",
	"near", "over", "toward", "under", "worth",
};

static std::string conjunctive_list[7] = {
	"because", "before", "either", "neither", "since", "that", "until",
};

template<typename T1>
inline uint256 HashSDKPGABSPCSSWS_EVEN(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B, uint32_t sinetable_pos)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), SDKPGABSPC_sinetable[sinetable_pos] * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	std::string str = "";
	uint32_t p = 0;
	uint32_t rnd;

	while(str.length()<121){
		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(adjective_list[rnd%323]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(noun_list[rnd%1222]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(verb_list[rnd%433]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p];
		str.append(adverb_list[rnd%48]);//.c_str());
		str.append(" ");
		p++;

		rnd = hash[0].begin()[p];
		if(rnd<32){
			str.append(preposition_list[rnd%15]);//.c_str());
			str.append(" ");
		} else if(rnd>=224){
			str.append(conjunctive_list[rnd%7]);//.c_str());
			str.append(" ");
		}
		p++;
	}

	const char *cstr = str.c_str();

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&cstr[0]), 121);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64-SDKPGABSPC_sinetable[sinetable_pos]);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	hash[1] =~hash[1];

	temp_hash = hash[1];
	hash[1] = temp_hash>>B;
	temp_hash <<=(512-B);
	hash[1] += temp_hash;

	return hash[1].trim256();
}


template<typename T1>
inline uint256 HashSDKPGABSPCSSWS_ODD(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B, uint32_t sinetable_pos)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), SDKPGABSPC_sinetable[sinetable_pos] * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	hash[0] =~hash[0];

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	std::string str = "";
	uint32_t p = 0;
	uint32_t rnd;

	while(str.length()<121){
		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(adjective_list[rnd%323]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(noun_list[rnd%1222]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(verb_list[rnd%433]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p];
		str.append(adverb_list[rnd%48]);//.c_str());
		str.append(" ");
		p++;

		rnd = hash[0].begin()[p];
		if(rnd<32){
			str.append(preposition_list[rnd%15]);//.c_str());
			str.append(" ");
		} else if(rnd>=224){
			str.append(conjunctive_list[rnd%7]);//.c_str());
			str.append(" ");
		}
		p++;
	}

	const char *cstr = str.c_str();

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&cstr[0]), 121);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64-SDKPGABSPC_sinetable[sinetable_pos]);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	temp_hash = hash[1];
	hash[1] = temp_hash>>B;
	temp_hash <<=(512-B);
	hash[1] += temp_hash;

	return hash[1].trim256();
}


template<typename T1>
inline std::string GetWordSalad_SDKPGABSPCSSWS_EVEN(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B, uint32_t sinetable_pos)

{
	sph_keccak512_context    ctx_keccak;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), SDKPGABSPC_sinetable[sinetable_pos] * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	std::string str = "";
	uint32_t p = 0;
	uint32_t rnd;

	while(str.length()<121){
		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(adjective_list[rnd%323]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(noun_list[rnd%1222]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(verb_list[rnd%433]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p];
		str.append(adverb_list[rnd%48]);//.c_str());
		str.append(" ");
		p++;

		rnd = hash[0].begin()[p];
		if(rnd<32){
			str.append(preposition_list[rnd%15]);//.c_str());
			str.append(" ");
		} else if(rnd>=224){
			str.append(conjunctive_list[rnd%7]);//.c_str());
			str.append(" ");
		}
		p++;
	}

	return str;
}

template<typename T1>
inline std::string GetWordSalad_SDKPGABSPCSSWS_ODD(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B, uint32_t sinetable_pos)

{
	sph_keccak512_context    ctx_keccak;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), SDKPGABSPC_sinetable[sinetable_pos] * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	hash[0] =~hash[0];

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	std::string str = "";
	uint32_t p = 0;
	uint32_t rnd;

	while(str.length()<121){
		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(adjective_list[rnd%323]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(noun_list[rnd%1222]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(verb_list[rnd%433]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p];
		str.append(adverb_list[rnd%48]);//.c_str());
		str.append(" ");
		p++;

		rnd = hash[0].begin()[p];
		if(rnd<32){
			str.append(preposition_list[rnd%15]);//.c_str());
			str.append(" ");
		} else if(rnd>=224){
			str.append(conjunctive_list[rnd%7]);//.c_str());
			str.append(" ");
		}
		p++;
	}

	return str;
}








template<typename T1>
inline uint256 HashSDKPGABSPCSSWSSBP_EVEN(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B, uint32_t sinetable_pos, uint256 pubkey_hashPrevBlock)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), SDKPGABSPC_sinetable[sinetable_pos] * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	std::string str = "";
	uint32_t p = 0;
	uint32_t rnd;

	while(str.length()<121){
		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(adjective_list[rnd%323]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(noun_list[rnd%1222]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(verb_list[rnd%433]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p];
		str.append(adverb_list[rnd%48]);//.c_str());
		str.append(" ");
		p++;

		rnd = hash[0].begin()[p];
		if(rnd<32){
			str.append(preposition_list[rnd%15]);//.c_str());
			str.append(" ");
		} else if(rnd>=224){
			str.append(conjunctive_list[rnd%7]);//.c_str());
			str.append(" ");
		}
		p++;
	}

	const char *cstr = str.c_str();

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&cstr[0]), 121);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&pubkey_hashPrevBlock), 32);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64-SDKPGABSPC_sinetable[sinetable_pos]);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	hash[1] =~hash[1];

	temp_hash = hash[1];
	hash[1] = temp_hash>>B;
	temp_hash <<=(512-B);
	hash[1] += temp_hash;

	return hash[1].trim256();
}


template<typename T1>
inline uint256 HashSDKPGABSPCSSWSSBP_ODD(const T1 pbegin, const T1 pend, uint8_t A, uint8_t B, uint32_t sinetable_pos, uint256 pubkey_hashPrevBlock)

{
	sph_keccak512_context    ctx_keccak;
	sph_keccak512_context    ctx_keccak2;
	static unsigned char pblank[1];

	uint512 temp_hash;
	uint512 hash[2];

	sph_keccak512_init(&ctx_keccak);
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
	sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), SDKPGABSPC_sinetable[sinetable_pos] * sizeof(pbegin[0]));
	sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

	hash[0] =~hash[0];

	temp_hash = hash[0];
	hash[0] = temp_hash<<A;
	temp_hash >>=(512-A);
	hash[0] += temp_hash;

	std::string str = "";
	uint32_t p = 0;
	uint32_t rnd;

	while(str.length()<121){
		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(adjective_list[rnd%323]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(noun_list[rnd%1222]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p]+(hash[0].begin()[p+1]<<8);
		str.append(verb_list[rnd%433]);//.c_str());
		str.append(" ");
		p+=2;

		rnd = hash[0].begin()[p];
		str.append(adverb_list[rnd%48]);//.c_str());
		str.append(" ");
		p++;

		rnd = hash[0].begin()[p];
		if(rnd<32){
			str.append(preposition_list[rnd%15]);//.c_str());
			str.append(" ");
		} else if(rnd>=224){
			str.append(conjunctive_list[rnd%7]);//.c_str());
			str.append(" ");
		}
		p++;
	}

	const char *cstr = str.c_str();

	sph_keccak512_init(&ctx_keccak2);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&cstr[0]), 121);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&pubkey_hashPrevBlock), 32);
	sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64-SDKPGABSPC_sinetable[sinetable_pos]);
	sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

	temp_hash = hash[1];
	hash[1] = temp_hash>>B;
	temp_hash <<=(512-B);
	hash[1] += temp_hash;

	return hash[1].trim256();
}













#endif // POW_H
