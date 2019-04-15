#ifndef POW_H
#define POW_H

#include "uint256.h"
#include "sph_keccak.h"


//Burgerhash V1 --- SpreadDoubleKetchup

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


//Burgerhash V2 --- SpreadDoubleKetchupPrimeGradeABeef

template<typename T1>
inline uint256 HashSDKPGAB(const T1 pbegin, const T1 pend,
                           bool nHeight_even,
                           uint8_t A, uint8_t B)
{
    sph_keccak512_context    ctx_keccak;
    sph_keccak512_context    ctx_keccak2;
    static unsigned char pblank[1];

    uint512 temp_hash;
    uint512 hash[2];

    sph_keccak512_init(&ctx_keccak);
    sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
    sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

    if(!nHeight_even){
        hash[0] =~hash[0];
    }

    temp_hash = hash[0];
    hash[0] = temp_hash<<A;
    temp_hash >>=(512-A);
    hash[0] += temp_hash;

    sph_keccak512_init(&ctx_keccak2);
    sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
    sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

    if(nHeight_even){
        hash[1] =~hash[1];
    }

    temp_hash = hash[1];
    hash[1] = temp_hash>>B;
    temp_hash <<=(512-B);
    hash[1] += temp_hash;

    return hash[1].trim256();
}



//Burgerhash V3 --- SpreadDoubleKetchupPrimeGradeABeefStickyPuffyCheese

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
inline uint256 HashSDKPGABSPC(const T1 pbegin, const T1 pend,
                              bool nHeight_even,
                              uint8_t A, uint8_t B,
                              uint32_t sinetable_pos)
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

    if(!nHeight_even){
        hash[0] =~hash[0];
    }

    temp_hash = hash[0];
    hash[0] = temp_hash<<A;
    temp_hash >>=(512-A);
    hash[0] += temp_hash;

    sph_keccak512_init(&ctx_keccak2);
    sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64);
    sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64-SDKPGABSPC_sinetable[sinetable_pos]);
    sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

    if(nHeight_even){
        hash[1] =~hash[1];
    }

    temp_hash = hash[1];
    hash[1] = temp_hash>>B;
    temp_hash <<=(512-B);
    hash[1] += temp_hash;

    return hash[1].trim256();
}



//Burgerhash V4 --- SpreadDoubleKetchupPrimeGradeABeefStickyPuffyCheeseSomethingSomethingWordSalad

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
inline uint256 HashSDKPGABSPCSSWS(const T1 pbegin, const T1 pend,
                                  bool nHeight_even,
                                  uint8_t A, uint8_t B,
                                  uint32_t sinetable_pos)
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

    if(!nHeight_even){
        hash[0] =~hash[0];
    }

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

    if(nHeight_even){
        hash[1] =~hash[1];
    }

    temp_hash = hash[1];
    hash[1] = temp_hash>>B;
    temp_hash <<=(512-B);
    hash[1] += temp_hash;

    return hash[1].trim256();
}


template<typename T1>
inline std::string GetWordSalad_SDKPGABSPCSSWS(const T1 pbegin, const T1 pend,
                                               bool nHeight_even,
                                               uint8_t A, uint8_t B,
                                               uint32_t sinetable_pos)
{
    sph_keccak512_context    ctx_keccak;
    static unsigned char pblank[1];

    uint512 temp_hash;
    uint512 hash[2];

    sph_keccak512_init(&ctx_keccak);
    sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
    sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), SDKPGABSPC_sinetable[sinetable_pos] * sizeof(pbegin[0]));
    sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

    if(!nHeight_even){
        hash[0] =~hash[0];
    }

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


//Burgerhash V5 --- SpreadDoubleKetchupPrimeGradeABeefStickyPuffyCheeseSomethingSomethingWordSaladSenoritaBonitaPepita

template<typename T1>
inline uint256 HashSDKPGABSPCSSWSSBP(const T1 pbegin, const T1 pend,
                                     bool nHeight_even,
                                     uint8_t A, uint8_t B,
                                     uint32_t sinetable_pos,
                                     uint256 pubkey_hashPrevBlock)
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

    if(!nHeight_even){
        hash[0] =~hash[0];
    }

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

    if(nHeight_even){
        hash[1] =~hash[1];
    }

    temp_hash = hash[1];
    hash[1] = temp_hash>>B;
    temp_hash <<=(512-B);
    hash[1] += temp_hash;

    return hash[1].trim256();
}




//Burgerhash V6 --- SpreadDoubleKetchupPrimeGradeABeefStickyPuffyCheeseSomethingSomethingWordSaladSenoritaBonitaPepita_ACELIGHT

static uint512 SDKPGABSPCSSWSSBP_ACELIGHT_A_B_FILLER[] =
{
    (uint512)"00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    (uint512)"01010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101",
    (uint512)"02020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202",
    (uint512)"03030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303",
    (uint512)"04040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404",
    (uint512)"05050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505",
    (uint512)"06060606060606060606060606060606060606060606060606060606060606060606060606060606060606060606060606060606060606060606060606060606",
    (uint512)"07070707070707070707070707070707070707070707070707070707070707070707070707070707070707070707070707070707070707070707070707070707",
    (uint512)"08080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808",
    (uint512)"09090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909",
    (uint512)"0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a",
    (uint512)"0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b",
    (uint512)"0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c",
    (uint512)"0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d",
    (uint512)"0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e",
    (uint512)"0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f",
    (uint512)"10101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010",
    (uint512)"11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111",
    (uint512)"12121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212",
    (uint512)"13131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313",
    (uint512)"14141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414",
    (uint512)"15151515151515151515151515151515151515151515151515151515151515151515151515151515151515151515151515151515151515151515151515151515",
    (uint512)"16161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616",
    (uint512)"17171717171717171717171717171717171717171717171717171717171717171717171717171717171717171717171717171717171717171717171717171717",
    (uint512)"18181818181818181818181818181818181818181818181818181818181818181818181818181818181818181818181818181818181818181818181818181818",
    (uint512)"19191919191919191919191919191919191919191919191919191919191919191919191919191919191919191919191919191919191919191919191919191919",
    (uint512)"1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a",
    (uint512)"1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b",
    (uint512)"1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c",
    (uint512)"1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d",
    (uint512)"1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e",
    (uint512)"1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f",
    (uint512)"20202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020",
    (uint512)"21212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121212121",
    (uint512)"22222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222",
    (uint512)"23232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323",
    (uint512)"24242424242424242424242424242424242424242424242424242424242424242424242424242424242424242424242424242424242424242424242424242424",
    (uint512)"25252525252525252525252525252525252525252525252525252525252525252525252525252525252525252525252525252525252525252525252525252525",
    (uint512)"26262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626",
    (uint512)"27272727272727272727272727272727272727272727272727272727272727272727272727272727272727272727272727272727272727272727272727272727",
    (uint512)"28282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828",
    (uint512)"29292929292929292929292929292929292929292929292929292929292929292929292929292929292929292929292929292929292929292929292929292929",
    (uint512)"2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a",
    (uint512)"2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b",
    (uint512)"2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c",
    (uint512)"2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d",
    (uint512)"2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e",
    (uint512)"2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f",
    (uint512)"30303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030303030",
    (uint512)"31313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131313131",
    (uint512)"32323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232",
    (uint512)"33333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333",
    (uint512)"34343434343434343434343434343434343434343434343434343434343434343434343434343434343434343434343434343434343434343434343434343434",
    (uint512)"35353535353535353535353535353535353535353535353535353535353535353535353535353535353535353535353535353535353535353535353535353535",
    (uint512)"36363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636",
    (uint512)"37373737373737373737373737373737373737373737373737373737373737373737373737373737373737373737373737373737373737373737373737373737",
    (uint512)"38383838383838383838383838383838383838383838383838383838383838383838383838383838383838383838383838383838383838383838383838383838",
    (uint512)"39393939393939393939393939393939393939393939393939393939393939393939393939393939393939393939393939393939393939393939393939393939",
    (uint512)"3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a",
    (uint512)"3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b",
    (uint512)"3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c",
    (uint512)"3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d",
    (uint512)"3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e",
    (uint512)"3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f",
    (uint512)"40404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040404040",
    (uint512)"41414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141",
    (uint512)"42424242424242424242424242424242424242424242424242424242424242424242424242424242424242424242424242424242424242424242424242424242",
    (uint512)"43434343434343434343434343434343434343434343434343434343434343434343434343434343434343434343434343434343434343434343434343434343",
    (uint512)"44444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444444",
    (uint512)"45454545454545454545454545454545454545454545454545454545454545454545454545454545454545454545454545454545454545454545454545454545",
    (uint512)"46464646464646464646464646464646464646464646464646464646464646464646464646464646464646464646464646464646464646464646464646464646",
    (uint512)"47474747474747474747474747474747474747474747474747474747474747474747474747474747474747474747474747474747474747474747474747474747",
    (uint512)"48484848484848484848484848484848484848484848484848484848484848484848484848484848484848484848484848484848484848484848484848484848",
    (uint512)"49494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949",
    (uint512)"4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a",
    (uint512)"4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b",
    (uint512)"4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c",
    (uint512)"4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d",
    (uint512)"4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e",
    (uint512)"4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f",
    (uint512)"50505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050505050",
    (uint512)"51515151515151515151515151515151515151515151515151515151515151515151515151515151515151515151515151515151515151515151515151515151",
    (uint512)"52525252525252525252525252525252525252525252525252525252525252525252525252525252525252525252525252525252525252525252525252525252",
    (uint512)"53535353535353535353535353535353535353535353535353535353535353535353535353535353535353535353535353535353535353535353535353535353",
    (uint512)"54545454545454545454545454545454545454545454545454545454545454545454545454545454545454545454545454545454545454545454545454545454",
    (uint512)"55555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555555",
    (uint512)"56565656565656565656565656565656565656565656565656565656565656565656565656565656565656565656565656565656565656565656565656565656",
    (uint512)"57575757575757575757575757575757575757575757575757575757575757575757575757575757575757575757575757575757575757575757575757575757",
    (uint512)"58585858585858585858585858585858585858585858585858585858585858585858585858585858585858585858585858585858585858585858585858585858",
    (uint512)"59595959595959595959595959595959595959595959595959595959595959595959595959595959595959595959595959595959595959595959595959595959",
    (uint512)"5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a",
    (uint512)"5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b",
    (uint512)"5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c",
    (uint512)"5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d",
    (uint512)"5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e",
    (uint512)"5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f",
    (uint512)"60606060606060606060606060606060606060606060606060606060606060606060606060606060606060606060606060606060606060606060606060606060",
    (uint512)"61616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161",
    (uint512)"62626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262626262",
    (uint512)"63636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363636363",
    (uint512)"64646464646464646464646464646464646464646464646464646464646464646464646464646464646464646464646464646464646464646464646464646464",
    (uint512)"65656565656565656565656565656565656565656565656565656565656565656565656565656565656565656565656565656565656565656565656565656565",
    (uint512)"66666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666",
    (uint512)"67676767676767676767676767676767676767676767676767676767676767676767676767676767676767676767676767676767676767676767676767676767",
    (uint512)"68686868686868686868686868686868686868686868686868686868686868686868686868686868686868686868686868686868686868686868686868686868",
    (uint512)"69696969696969696969696969696969696969696969696969696969696969696969696969696969696969696969696969696969696969696969696969696969",
    (uint512)"6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a",
    (uint512)"6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b",
    (uint512)"6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c",
    (uint512)"6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d",
    (uint512)"6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e",
    (uint512)"6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f",
    (uint512)"70707070707070707070707070707070707070707070707070707070707070707070707070707070707070707070707070707070707070707070707070707070",
    (uint512)"71717171717171717171717171717171717171717171717171717171717171717171717171717171717171717171717171717171717171717171717171717171",
    (uint512)"72727272727272727272727272727272727272727272727272727272727272727272727272727272727272727272727272727272727272727272727272727272",
    (uint512)"73737373737373737373737373737373737373737373737373737373737373737373737373737373737373737373737373737373737373737373737373737373",
    (uint512)"74747474747474747474747474747474747474747474747474747474747474747474747474747474747474747474747474747474747474747474747474747474",
    (uint512)"75757575757575757575757575757575757575757575757575757575757575757575757575757575757575757575757575757575757575757575757575757575",
    (uint512)"76767676767676767676767676767676767676767676767676767676767676767676767676767676767676767676767676767676767676767676767676767676",
    (uint512)"77777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777",
    (uint512)"78787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878",
    (uint512)"79797979797979797979797979797979797979797979797979797979797979797979797979797979797979797979797979797979797979797979797979797979",
    (uint512)"7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a",
    (uint512)"7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b",
    (uint512)"7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c",
    (uint512)"7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d",
    (uint512)"7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e",
    (uint512)"7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f",
    (uint512)"80808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080808080",
    (uint512)"81818181818181818181818181818181818181818181818181818181818181818181818181818181818181818181818181818181818181818181818181818181",
    (uint512)"82828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282828282",
    (uint512)"83838383838383838383838383838383838383838383838383838383838383838383838383838383838383838383838383838383838383838383838383838383",
    (uint512)"84848484848484848484848484848484848484848484848484848484848484848484848484848484848484848484848484848484848484848484848484848484",
    (uint512)"85858585858585858585858585858585858585858585858585858585858585858585858585858585858585858585858585858585858585858585858585858585",
    (uint512)"86868686868686868686868686868686868686868686868686868686868686868686868686868686868686868686868686868686868686868686868686868686",
    (uint512)"87878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787878787",
    (uint512)"88888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888",
    (uint512)"89898989898989898989898989898989898989898989898989898989898989898989898989898989898989898989898989898989898989898989898989898989",
    (uint512)"8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a",
    (uint512)"8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b",
    (uint512)"8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c",
    (uint512)"8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d",
    (uint512)"8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e",
    (uint512)"8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f",
    (uint512)"90909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090909090",
    (uint512)"91919191919191919191919191919191919191919191919191919191919191919191919191919191919191919191919191919191919191919191919191919191",
    (uint512)"92929292929292929292929292929292929292929292929292929292929292929292929292929292929292929292929292929292929292929292929292929292",
    (uint512)"93939393939393939393939393939393939393939393939393939393939393939393939393939393939393939393939393939393939393939393939393939393",
    (uint512)"94949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494949494",
    (uint512)"95959595959595959595959595959595959595959595959595959595959595959595959595959595959595959595959595959595959595959595959595959595",
    (uint512)"96969696969696969696969696969696969696969696969696969696969696969696969696969696969696969696969696969696969696969696969696969696",
    (uint512)"97979797979797979797979797979797979797979797979797979797979797979797979797979797979797979797979797979797979797979797979797979797",
    (uint512)"98989898989898989898989898989898989898989898989898989898989898989898989898989898989898989898989898989898989898989898989898989898",
    (uint512)"99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999",
    (uint512)"9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a",
    (uint512)"9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b",
    (uint512)"9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c",
    (uint512)"9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d",
    (uint512)"9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e",
    (uint512)"9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f",
    (uint512)"a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0",
    (uint512)"a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1a1",
    (uint512)"a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2",
    (uint512)"a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3a3",
    (uint512)"a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4a4",
    (uint512)"a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5",
    (uint512)"a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6a6",
    (uint512)"a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7a7",
    (uint512)"a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8a8",
    (uint512)"a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9a9",
    (uint512)"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    (uint512)"abababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababab",
    (uint512)"acacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacac",
    (uint512)"adadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadad",
    (uint512)"aeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeae",
    (uint512)"afafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafaf",
    (uint512)"b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0",
    (uint512)"b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1b1",
    (uint512)"b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2b2",
    (uint512)"b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3b3",
    (uint512)"b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4b4",
    (uint512)"b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5b5",
    (uint512)"b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6b6",
    (uint512)"b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7b7",
    (uint512)"b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8b8",
    (uint512)"b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9b9",
    (uint512)"babababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababa",
    (uint512)"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
    (uint512)"bcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbc",
    (uint512)"bdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbd",
    (uint512)"bebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebe",
    (uint512)"bfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbf",
    (uint512)"c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0c0",
    (uint512)"c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1c1",
    (uint512)"c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2c2",
    (uint512)"c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3c3",
    (uint512)"c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4c4",
    (uint512)"c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5c5",
    (uint512)"c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6",
    (uint512)"c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7c7",
    (uint512)"c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8c8",
    (uint512)"c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9c9",
    (uint512)"cacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacacaca",
    (uint512)"cbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcbcb",
    (uint512)"cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc",
    (uint512)"cdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcd",
    (uint512)"cececececececececececececececececececececececececececececececececececececececececececececececececececececececececececececececece",
    (uint512)"cfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcf",
    (uint512)"d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0d0",
    (uint512)"d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1d1",
    (uint512)"d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2d2",
    (uint512)"d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3d3",
    (uint512)"d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4d4",
    (uint512)"d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5d5",
    (uint512)"d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6d6",
    (uint512)"d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7d7",
    (uint512)"d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8d8",
    (uint512)"d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9d9",
    (uint512)"dadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadada",
    (uint512)"dbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdbdb",
    (uint512)"dcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdcdc",
    (uint512)"dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd",
    (uint512)"dededededededededededededededededededededededededededededededededededededededededededededededededededededededededededededededede",
    (uint512)"dfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdf",
    (uint512)"e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0e0",
    (uint512)"e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1e1",
    (uint512)"e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2e2",
    (uint512)"e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3e3",
    (uint512)"e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4e4",
    (uint512)"e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5",
    (uint512)"e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6e6",
    (uint512)"e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7e7",
    (uint512)"e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8e8",
    (uint512)"e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9e9",
    (uint512)"eaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaeaea",
    (uint512)"ebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebebeb",
    (uint512)"ecececececececececececececececececececececececececececececececececececececececececececececececececececececececececececececececec",
    (uint512)"edededededededededededededededededededededededededededededededededededededededededededededededededededededededededededededededed",
    (uint512)"eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
    (uint512)"efefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefef",
    (uint512)"f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0",
    (uint512)"f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1",
    (uint512)"f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2",
    (uint512)"f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3f3",
    (uint512)"f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4f4",
    (uint512)"f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5f5",
    (uint512)"f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6f6",
    (uint512)"f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7f7",
    (uint512)"f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8f8",
    (uint512)"f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9f9",
    (uint512)"fafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafa",
    (uint512)"fbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfbfb",
    (uint512)"fcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfcfc",
    (uint512)"fdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfd",
    (uint512)"fefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefefe",
    (uint512)"ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff",
};

static unsigned char SDKPGABSPCSSWSSBP_ACELIGHT_reverse_bits[] =
{
    0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
    0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
    0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
    0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
    0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
    0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
    0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
    0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
    0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
    0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
    0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
    0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
    0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
    0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
    0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
    0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff,
};


template<typename T1>
inline uint256 HashSDKPGABSPCSSWSSBP_ACELIGHT(const T1 pbegin, const T1 pend,
                                              bool nHeight_even,
                                              uint8_t A, uint8_t B,
                                              uint32_t sinetable_pos,
                                              uint256 pubkey_hashPrevBlock,
                                              std::vector<unsigned int> *ace_instructions, uint32_t _ace_instruction_count)
{
    sph_keccak512_context    ctx_keccak;
    sph_keccak512_context    ctx_keccak2;
    static unsigned char pblank[1];

    uint512 temp_hash;
    uint512 hash[2];

    unsigned char temp_char;
    unsigned char ACELIGHT_METHOD;
    unsigned short ACELIGHT_BYTE_A;  //modulo 46
    unsigned short ACELIGHT_BYTE_B;  //modulo 46
    unsigned short ACELIGHT_BYTE_A_FULL;
    unsigned short ACELIGHT_BYTE_B_FULL;
    unsigned short ACELIGHT_BIT;  //modulo 368

    unsigned char sine_size = SDKPGABSPC_sinetable[sinetable_pos];

    uint384 hashWholeBlockAndMore;
    hashWholeBlockAndMore.SetHex(HexStr(&pbegin[88], &pbegin[88+46]).c_str());

    const int ace_instruction_count = _ace_instruction_count;

    //ACE-LIGHT calculations:

    for (int i = 0; i<ace_instruction_count;i++){

        uint32_t instruction = ace_instructions->at(i);
        ACELIGHT_METHOD = (((unsigned char*)&instruction)[0])>>5;
        ACELIGHT_BYTE_A_FULL = ((unsigned char*)&instruction)[1];
        ACELIGHT_BYTE_B_FULL = ((unsigned char*)&instruction)[2];
        ACELIGHT_BYTE_A = ACELIGHT_BYTE_A_FULL%46;
        ACELIGHT_BYTE_B = ACELIGHT_BYTE_B_FULL%46;

        if(ACELIGHT_METHOD == 0){
            //Swap 2 bytes
            if(ACELIGHT_BYTE_A == ACELIGHT_BYTE_B){
                (ACELIGHT_BYTE_B == 45) ? ACELIGHT_BYTE_B = 0 : ACELIGHT_BYTE_B++;
            }
            temp_char = hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A];
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] = hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B];
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] = temp_char;
        }

        else if(ACELIGHT_METHOD == 1){
            //Left Rotate 2 bytes
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] = (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] & 0x80)
                    ? (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A]<<1)^0x01
                    : hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A]<<1;
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] = (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] & 0x80)
                    ? (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B]<<1)^0x01
                    : hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B]<<1;
        }

        else if(ACELIGHT_METHOD == 2){
            //Right Rotate 2 bytes
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] = (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] & 0x01)
                    ? (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A]>>1)^0x80
                    : hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A]>>1;
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] = (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] & 0x01)
                    ? (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B]>>1)^0x80
                    : hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B]>>1;
        }

        else if(ACELIGHT_METHOD == 3){
            //Invert 2 bytes
            if(ACELIGHT_BYTE_A == ACELIGHT_BYTE_B){
                (ACELIGHT_BYTE_B == 45) ? ACELIGHT_BYTE_B = 0 : ACELIGHT_BYTE_B++;
            }
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] = ~hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A];
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] = ~hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B];
        }

        else if(ACELIGHT_METHOD == 4){
            //set 1 bit from whole 368 bites
            ACELIGHT_BIT = ((ACELIGHT_BYTE_A_FULL<<8) + ACELIGHT_BYTE_B_FULL) % 368;
            hashWholeBlockAndMore.begin()[ACELIGHT_BIT/8] = (hashWholeBlockAndMore.begin()[ACELIGHT_BIT/8]&~(1<<(ACELIGHT_BIT%8))) | (1<<(ACELIGHT_BIT%8));
        }

        else if(ACELIGHT_METHOD == 5){
            //unset 1 bit from whole 368 bites
            ACELIGHT_BIT = ((ACELIGHT_BYTE_A_FULL<<8) + ACELIGHT_BYTE_B_FULL) % 368;
            hashWholeBlockAndMore.begin()[ACELIGHT_BIT/8] = (hashWholeBlockAndMore.begin()[ACELIGHT_BIT/8]&~(1<<(ACELIGHT_BIT%8))) | (0<<(ACELIGHT_BIT%8));
        }

        else if(ACELIGHT_METHOD == 6){
            //invert single bit
            ACELIGHT_BIT = ((ACELIGHT_BYTE_A_FULL<<8) + ACELIGHT_BYTE_B_FULL) % 368;
            hashWholeBlockAndMore.begin()[ACELIGHT_BIT/8] = (hashWholeBlockAndMore.begin()[ACELIGHT_BIT/8]^(1<<(ACELIGHT_BIT%8)));
        }

        else if(ACELIGHT_METHOD == 7){
            //Reverse 2 bytes
            if(ACELIGHT_BYTE_A == ACELIGHT_BYTE_B){
                (ACELIGHT_BYTE_B == 45) ? ACELIGHT_BYTE_B = 0 : ACELIGHT_BYTE_B++;
            }
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] = SDKPGABSPCSSWSSBP_ACELIGHT_reverse_bits[hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A]];
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] = SDKPGABSPCSSWSSBP_ACELIGHT_reverse_bits[hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B]];
        }

   }

    sph_keccak512_init(&ctx_keccak);
    sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
    sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), sine_size * sizeof(pbegin[0]));
    sph_keccak512 (&ctx_keccak, static_cast<const void*>(&SDKPGABSPCSSWSSBP_ACELIGHT_A_B_FILLER[A]), 64-sine_size);
    sph_keccak512 (&ctx_keccak, static_cast<const void*>(&hashWholeBlockAndMore), 39);
    sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

    if(!nHeight_even){
        hash[0] =~hash[0];
    }

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
    sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hash[0]), 64-sine_size);
    sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&SDKPGABSPCSSWSSBP_ACELIGHT_A_B_FILLER[B]), sine_size);
    sph_keccak512 (&ctx_keccak2, static_cast<const void*>(&hashWholeBlockAndMore.begin()[39]), 7);
    sph_keccak512_close(&ctx_keccak2, static_cast<void*>(&hash[1]));

    if(nHeight_even){
        hash[1] =~hash[1];
    }

    temp_hash = hash[1];
    hash[1] = temp_hash>>B;
    temp_hash <<=(512-B);
    hash[1] += temp_hash;

    return hash[1].trim256();
}


template<typename T1>
inline std::string GetWordSalad_SDKPGABSPCSSWSSBP_ACELIGHT(const T1 pbegin, const T1 pend,
                                              bool nHeight_even,
                                              uint8_t A, uint8_t B,
                                              uint32_t sinetable_pos,
                                              std::vector<unsigned int> *ace_instructions, uint32_t _ace_instruction_count)
{
    sph_keccak512_context    ctx_keccak;
    //sph_keccak512_context    ctx_keccak2;
    static unsigned char pblank[1];

    uint512 temp_hash;
    uint512 hash[2];

    unsigned char temp_char;
    unsigned char ACELIGHT_METHOD;
    unsigned short ACELIGHT_BYTE_A;  //modulo 46
    unsigned short ACELIGHT_BYTE_B;  //modulo 46
    unsigned short ACELIGHT_BYTE_A_FULL;
    unsigned short ACELIGHT_BYTE_B_FULL;
    unsigned short ACELIGHT_BIT;  //modulo 368

    unsigned char sine_size = SDKPGABSPC_sinetable[sinetable_pos];

    uint384 hashWholeBlockAndMore;
    hashWholeBlockAndMore.SetHex(HexStr(&pbegin[88], &pbegin[88+46]).c_str());

    const int ace_instruction_count = _ace_instruction_count;

    //ACE-LIGHT calculations:

    for (int i = 0; i<ace_instruction_count;i++){

        uint32_t instruction = ace_instructions->at(i);
        ACELIGHT_METHOD = (((unsigned char*)&instruction)[0])>>5;
        ACELIGHT_BYTE_A_FULL = ((unsigned char*)&instruction)[1];
        ACELIGHT_BYTE_B_FULL = ((unsigned char*)&instruction)[2];
        ACELIGHT_BYTE_A = ACELIGHT_BYTE_A_FULL%46;
        ACELIGHT_BYTE_B = ACELIGHT_BYTE_B_FULL%46;

        if(ACELIGHT_METHOD == 0){
            //Swap 2 bytes
            if(ACELIGHT_BYTE_A == ACELIGHT_BYTE_B){
                (ACELIGHT_BYTE_B == 45) ? ACELIGHT_BYTE_B = 0 : ACELIGHT_BYTE_B++;
            }
            temp_char = hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A];
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] = hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B];
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] = temp_char;
        }

        else if(ACELIGHT_METHOD == 1){
            //Left Rotate 2 bytes
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] = (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] & 0x80)
                    ? (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A]<<1)^0x01
                    : hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A]<<1;
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] = (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] & 0x80)
                    ? (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B]<<1)^0x01
                    : hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B]<<1;
        }

        else if(ACELIGHT_METHOD == 2){
            //Right Rotate 2 bytes
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] = (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] & 0x01)
                    ? (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A]>>1)^0x80
                    : hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A]>>1;
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] = (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] & 0x01)
                    ? (hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B]>>1)^0x80
                    : hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B]>>1;
        }

        else if(ACELIGHT_METHOD == 3){
            //Invert 2 bytes
            if(ACELIGHT_BYTE_A == ACELIGHT_BYTE_B){
                (ACELIGHT_BYTE_B == 45) ? ACELIGHT_BYTE_B = 0 : ACELIGHT_BYTE_B++;
            }
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] = ~hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A];
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] = ~hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B];
        }

        else if(ACELIGHT_METHOD == 4){
            //set 1 bit from whole 368 bites
            ACELIGHT_BIT = ((ACELIGHT_BYTE_A_FULL<<8) + ACELIGHT_BYTE_B_FULL) % 368;
            hashWholeBlockAndMore.begin()[ACELIGHT_BIT/8] = (hashWholeBlockAndMore.begin()[ACELIGHT_BIT/8]&~(1<<(ACELIGHT_BIT%8))) | (1<<(ACELIGHT_BIT%8));
        }

        else if(ACELIGHT_METHOD == 5){
            //unset 1 bit from whole 368 bites
            ACELIGHT_BIT = ((ACELIGHT_BYTE_A_FULL<<8) + ACELIGHT_BYTE_B_FULL) % 368;
            hashWholeBlockAndMore.begin()[ACELIGHT_BIT/8] = (hashWholeBlockAndMore.begin()[ACELIGHT_BIT/8]&~(1<<(ACELIGHT_BIT%8))) | (0<<(ACELIGHT_BIT%8));
        }

        else if(ACELIGHT_METHOD == 6){
            //invert single bit
            ACELIGHT_BIT = ((ACELIGHT_BYTE_A_FULL<<8) + ACELIGHT_BYTE_B_FULL) % 368;
            hashWholeBlockAndMore.begin()[ACELIGHT_BIT/8] = (hashWholeBlockAndMore.begin()[ACELIGHT_BIT/8]^(1<<(ACELIGHT_BIT%8)));
        }

        else if(ACELIGHT_METHOD == 7){
            //Reverse 2 bytes
            if(ACELIGHT_BYTE_A == ACELIGHT_BYTE_B){
                (ACELIGHT_BYTE_B == 45) ? ACELIGHT_BYTE_B = 0 : ACELIGHT_BYTE_B++;
            }
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A] = SDKPGABSPCSSWSSBP_ACELIGHT_reverse_bits[hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_A]];
            hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B] = SDKPGABSPCSSWSSBP_ACELIGHT_reverse_bits[hashWholeBlockAndMore.begin()[ACELIGHT_BYTE_B]];
        }

   }

    sph_keccak512_init(&ctx_keccak);
    sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
    sph_keccak512 (&ctx_keccak, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), sine_size * sizeof(pbegin[0]));
    sph_keccak512 (&ctx_keccak, static_cast<const void*>(&SDKPGABSPCSSWSSBP_ACELIGHT_A_B_FILLER[A]), 64-sine_size);
    sph_keccak512 (&ctx_keccak, static_cast<const void*>(&hashWholeBlockAndMore), 39);
    sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[0]));

    if(!nHeight_even){
        hash[0] =~hash[0];
    }

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





#endif // POW_H
