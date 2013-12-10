/*
 * LevelHelper.cpp
 *
 *  Created on: Nov 21, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#include "LevelHelper.h"

std::string LevelHelper::getDescriptionForLevel(int level) {

	std::string description = "";
	switch (level) {
	case 0:
		description =
				"While recovering from the excesses of New Year's Eve, we might"
						"ponder the enzyme that ceaselessly battles the champagne that we consume. "
						"Alcohol dehydrogenase is our primary defense against alcohol, "
						"a toxic molecule that compromises the function of our nervous system."
						"Alcohol dehydrogenase (EC:1.1.1.1) (ADH) catalyzes the reversible oxidation"
						"of alcohols to their corresponding acetaldehyde or ketone with the concomitant reduction of NAD";
		break;
	case 1:
		description =
				"p53 tumor suppressor is a transcription factor that mobilizes genes involved in sensing and responding to cell damage. It is a large, flexible molecule composed of four chains. This structure includes the portion that links the four chains together.";
		break;
	case 2:
		description =
				"Carbon is essential to life. All of our molecular machines are built around a central scaffolding of organic carbon. Unfortunately, carbon in the earth and atmosphere is locked in highly oxidized forms, such as carbonate minerals and carbon dioxide gas.";
		break;
	case 3:
		description =
				"Proteins are tough, so we use an arsenal of enzymes to digest them into their component amino acids. Digestion of proteins begins in the stomach, where hydrochloric acid unfolds proteins and the enzyme pepsin begins a rough disassembly. The real work then starts in the intestines. The pancreas adds a collection of protein-cutting enzymes, with trypsin playing the central role, that chop the protein chains into pieces just a few amino acids long";
		break;
	case 4:
		description =
				" If one strand in the DNA breaks, it is not a disaster, but it can lead to problems when the DNA double helix is unwound during the processes of transcription and replication. Breakage of both strands, on the other hand, is far more serious. To protect us from these dangers, our cells use DNA ligases to glue together DNA strands that have been broken.";
		break;
	case 5:
		description =
				"DNA polymerase plays the central role in the processes of life. It carries the weighty responsibility of duplicating our genetic information. Each time a cell divides, DNA polymerase duplicates all of its DNA, and the cell passes one copy to each daughter cell. In this way, genetic information is passed from generation to generation";
		break;
	case 6:
		description =
				"RNA is a versatile molecule. In its most familiar role, RNA acts as an intermediary, carrying genetic information from the DNA to the machinery of protein synthesis. RNA also plays more active roles, performing many of the catalytic and recognition functions normally reserved for proteins. The enzyme RNA polymerase carries the weighty responsibility of creating all of these different RNA molecule";
		break;
	case 7:
		description =
				"DNA is normally only created using other DNA strands as a template. This tricky reversal of synthesis is performed by the enzyme reverse transcriptase, shown here from PDB entry 3hvt. Inside its large, claw-shaped active site, it copies the HIV RNA and creates a double-stranded DNA version of the viral genome. This then integrates into the cell's DNA, and later instructs the cell to make additional copies of the virus.";
		break;
	case 8:
		description =
				"When a ribosome pairs a 'CGC' tRNA with 'GCG' codon, it expects to find an alanine carried by the tRNA. It has no way of checking; each tRNA is matched with its amino acid long before it reaches the ribosome. The match is made by a collection of remarkable enzymes, the aminoacyl-tRNA synthetases. These enzymes charge each tRNA with the proper amino acid.";
		break;
	case 9:
		description =
				"Alpha-amylase begins the process of starch digestion. It takes starch chains and breaks them into smaller pieces with two or three glucose units.";
		break;
	case 10:
		description =
				"During the holiday season, we often place greater demands on our digestive enzymes than at other times of the year. Our digestive system contains a host of tough, stable enzymes designed to seek out those rich holiday treats and break them into small pieces. Pepsin is the first in a series of enzymes that digest proteins.";
		break;
	}

	/*
	 int i;
	 int lastbreak = 0;

	 int textWidth = 60;

	 for(i=0;i < description.size(); i++){
	 // after the first line end
	 if(i -lastbreak > textWidth){
	 int j;
	 // search for the last whitespace
	 for(j = textWidth; j >= 0 ; j--){
	 int pos = i -j+ textWidth;
	 if(description[pos] == ' '){
	 description = description.replace(pos, 1, "\n");
	 lastbreak = pos;
	 i = lastbreak;
	 break;
	 }
	 }
	 }
	 }

	 */

	return description;
}

std::string LevelHelper::getSequenceForLevel(int level) {
	std::string sequence = "";
	switch (level) {
	case 0:
		sequence =
				"STAGKVIKCKAAVLWEEKKPFSIEEVEVAPPKAHEVRIKMVATGICRSDDHVVSGTLVTPLPVIAGHEAAGIVESIGEGVTTVRPGDKVIPLFTPQCGKCRVCKHPEGNFCLKNDLSMPRGTMQDGTSRFTCRGKPIHHFLGTSTFSQYTVVDEISVAKIDAASPLEKVCLIGCGFSTGYGSAVKVAKVTQGSTCAVFGLGGVGLSVIMGCKAAGAARIIGVDINKDKFAKAKEVGATECVNPQDYKKPIQEVLTEMSNGGVDFSFEVIGRLDTMVTALSCCQEAYGVSVIVGVPPDSQNLSMNPMLLLSGRTWKGAIFGGFKSKDSVPKLVADFMAKKFALDPLITHVLPFEKINEGFDLLRSGESIRTILTF";
		break;
		// p53
	case 1:
		sequence = "KKKPLDGEYFTLQIRGRERFEMFRELNEALELKDAQAGKEPG";
		break;
	case 2:
		sequence =
				"MQVWPPINKKKYETLSYLPDLSQEQLLSEVEYLLKNGWVPCLEFETEHGFVYRENNKSPGYYDGRYWTMWKLPMFGCTDATQVLAEVEEAKKAYPQAWIRIIGFDNVRQVQCISFIAYKPEGY";
		break;
	case 3:
		sequence =
				"IVGGYTCGANTVPYQVSLNSGYHFCGGSLINSQWVVSAAHCYKSGIQVRLGEDNINVVEGNEQFISASKSIVHPSYNSNTLNNDIMLIKLKSAASLNSRVASISLPTSCASAGTQCLISGWGNTKSSGTSYPDVLKCLKAPILSDSSCKSAYPGQITSNMFCAGYLEGGKDSCQGDSGGPVVCSGKLQGIVSWGSGCAQKNKPGVYTKVCNYVSWIKQTIA";
		break;
	case 4:
		sequence =
				"MTREEARRRINELRDLIRYHNYRYYVLADPEISDAEYDRLLRELKELEERFPEFKSPDSPTEQVGARPLEPTFRPVRHPTRMYSLDNAFTYEEVLAFEERLEREAEAPSLYTVEHKVDGLSVLYYEEGVWSTGSGDGEVGEEVTQNLLTIPTIPRRLKGVPDRLEVRGEVYMPIEAFLRLNEELEERGEKVFKNPRNAAAGSLRQKDPRVTAKRGLRATFYALGLGLGLEESGLKSQYELLLWLKEKGFPVEHCYEKALGAEGVEEVYRRGLAQRHALPFEADGVVLKLDDLTLWGELGYTARAPRFALAYKFPAEEKETRLLDVVFQVGRTGRVTPVGVLEPVFIEGSEVSRVTLHNESYIEELDIRIGDWVLVHKAGGVIPEVLRVLKERRTGKERPIRWPEACPECGHRLVKEGKVHRCPNPLCPAKRFEAIRHYASRKAMDIEGLGEKLIERLLEKGLVRDVADLYHLRKEDLLGLERMGEKSAQNLLRQIEESKHRGLERLLYALGLPGVGEVLARNLARRFGTMDRLLEASLEELIEVEEVGELTARAILETLKDPAFRDLVRRLKEAGVSMESKEEVSDLLSGLTFVLTGELSRPREEVKALLGRLGAKVTDSVSRKTSYLVVGENPGSKLEKARALGVAVLTEEEFWRFLKEKGAPVPA";
		break;
	case 5:
		sequence =
				"VISYDNYVTILDEETLKAWIAKLEKAPVFAFATETDSLDNISANLVGLSFAIEPGVAAYIPVAHDYLDAPDQISRERALELLKPLLEDEKALKVGQNLKYDRGILANYGIELRGIAFDTMLESYILNSVAGRHDMDSLAERWLKHKTITFEEIAGKGKNQLTFNQIALEEAGRYAAEDADVTLQLHLKMWPDLQKHKGPLNVFENIEMPLVPVLSRIERNGVKIDPKVLHNHSEELTLRLAELEKKAHEIAGEEFNLSSTKQLQTILFEKQGIKPLKKTPGGAPSTSEEVLEELALDYPLPKVILEYRGLAKLKSTYTDKLPLMINPKTGRVHTSYHQAVTATGRLSSTDPNLQNIPVRNEEGRRIRQAFIAPEDYVIVSADYSQIELRIMAHLSRDKGLLTAFAEGKDIHRATAAEVFGLPLETVTSEQRRSAKAINFGLIYGMSAFGLARQLNIPRKEAQKYMDLYFERYPGVLEYMERTRAQAKEQGYVETLDGRRLYLPDIKSSNGARRAAAERAAINAPMQGTAADIIKRAMIAVDAWLQAEQPRVRMIMQVHDELVFEVHKDDVDAVAKQIHQLMENCTRLDVPLLVEVGSGENWDQAH";
		break;
	case 6:
		sequence =
				"MDQENERNISRLWRAFRTVKEMVKDRGYFITQEEVELPLEDFKAKYCDSMGRPQRKMMSFQANPTEESISKFPDMGSLWVEFCDEPSVGVKTMKTFVIHIQEKNFQTGIFVYQNNITPSAMKLVPSIPPATIETFNEAALVVNITHHELVPKHIRLSSDEKRELLKRYRLKESQLPRIQRADPVALYLGLKRGEVVKIIRKSETSGRYASYRICM";
		break;

	case 7:
		sequence =
				"PISPIETVPVKLKPGMDGPKVKQWPLTEEKIKALVEICTEMEKEGKISKIGPENPYNTPVFAIKKKDSTKWRKLVDFRELNKRTQDFWEVQLGIPHPAGLKKKKSVTVLDVGDAYFSVPLDEDFRKYTAFTIPSINNETPGIRYQYNVLPQGWKGSPAIFQSSMTKILEPFKKQNPDIVIYQYMDDLYVGSDLEIGQHRTKIEELRQHLLRWGLTTPDKKHQKEPPFLWMGYELHPDKWTVQPIVLPEKDSWTVNDIQKLVGKLNWASQIYPGIKVRQLSKLLRGTKALTEVIPLTEEAELELAENREILKEPVHGVYYDPSKDLIAEIQKQGQGQWTYQIYQEPFKNLKTGKYARMRGAHTNDVKQLTEAVQKITTESIVIWGKTPKFKLPIQKETWETWWTEYWQATWIPEWEFVNTPPLVKLWYQLEKEPIVGAETFYVDGAANRETKLGKAGYVTNKGRQKVVPLTNTTNQKTELQAIYLALQDSGLEVNIVTDSQYALGIIQAQPDKSESELVNQIIEQLIKKEKVYLAWVPAHKGIGGNEQVDKLVSAGIRK";
		break;

	case 8:
		sequence =
				"EDTAKDNYGKLPLIQSRDSDRTGQKRVKFVDLDEAKDSDKEVLFRARVHNTRQQGATLAFLTLRQQASLIQGLVKANKEGTISKNMVKWAGSLNLESIVLVRGIVKKVDEPIKSATVQNLEIHITKIYTISETPEALPILLEDASRSEAEAEAAGLPVVNLDTRLDYRVIDLRTVTNQAIFRIQAGVCELFREYLATKKFTEVHTPKLLGAPSEGGSSVFEVTYFKGKAYLAQSPQFNKQQLIVADFERVYEIGPVFRAENSNTHRHMTEFTGLDMEMAFEEHYHEVLDTLSELFVFIFSELPKRFAHEIELVRKQYPVEEFKLPKDGKMVRLTYKEGIEMLRAAGKEIGDFEDLSTENEKFLGKLVRDKYDTDFYILDKFPLEIRPFYTMPDPANPKYSNSYDFFMRGEEILSGAQRIHDHALLQERMKAHGLSPEDPGLKDYCDGFSYGCPPHAGGGIGLERVVMFYLDLKNIRRASLFPRDPKRLRP";
		break;

	case 9:
		sequence =
				"QYAPQTQSGRTSIVHLFEWRWVDIALECERYLGPKGFGGVQVSPPNENVVVTNPSRPWWERYQPVSYKLCTRSGNENEFRDMVTRCNNVGVRIYVDAVINHMCGSGAAAGTGTTCGSYCNPGSREFPAVPYSAWDFNDGKCKTASGGIESYNDPYQVRDCQLVGLLDLALEKDYVRSMIADYLNKLIDIGVAGFRIDASKHMWPGDIKAVLDKLHNLNTNWFPAGSRPFIFQEVIDLGGEAIKSSEYFGNGRVTEFKYGAKLGTVVRKWSGEKMSYLKNWGEGWGFMPSDRALVFVDNHDNQRGHGAGGSSILTFWDARLYKVAVGFMLAHPYGFTRVMSSYRWARNFVNGEDVNDWIGPPNNNGVIKEVTINADTTCGNDWVCEHRWREIRNMVWFRNVVDGEPFANWWDNGSNQVAFGRGNRGFIVFNNDDWQLSSTLQTGLPGGTYCDVISGDKVGNSCTGIKVYVSSDGTAQFSISNSAEDPFIAIHAESKL";
		break;
	case 10:
		sequence =
				"IGDEPLENYLDTEYFGTIGIGTPAQDFTVIFDTGSSNLWVPSVYCSSLACSDHNQFNPDDSSTFEATSQELSITYGTGSMTGILGYDTVQVGGISDTNQIFGLSETEPGSFLYYAPFDGILGLAYPSISASGATPVFDNLWDQGLVSQDLFSVYLSSNDDSGSVVLLGGIDSSYYTGSLNWVPVSVEGYWQITLDSITMDGETIACSGGCQAIVDTGTSLLTGPTSAIANIQSDIGASENSDGEMVISCSSIASLPDIVFTINGVQYPLSPSAYILQDDDSCTSGFEGMDVPTSSGELWILGDVFIRQYYTVFDRANNKVGLAPVA";
		break;
	default:
		sequence = "AAAAAAAAAAAAAAAAAAAAAAAAAA";
	}
	sequence = sequence.substr(0, 80);
	return sequence;
}

std::string LevelHelper::getCodeForLevel(int level) {
	std::string sequence = "";
	switch (level) {
	case 0:
		sequence = "1ADC";
		break;
	case 1:
		sequence = "1OLG";
		break;
	case 2:
		sequence = "1RLC";
		break;
	case 3:
		sequence = "2PTN";
		break;
	case 4:
		sequence = "1DGS";
		break;
	case 5:
		sequence = "1KLN";
		break;
	case 6:
		sequence = "2EI2I";
		break;

	case 7:
		sequence = "2HMI";
		break;

	case 8:
		sequence = "1ASZ";
		break;

	case 9:
		sequence = "1PPI";
		break;
	case 10:
		sequence = "5PEP";
		break;
	default:
		sequence = "";
	}
	return sequence;
}

std::string LevelHelper::getPathForLevel(int level) {
	std::string sequence = "";
	switch (level) {
	case 0:
		sequence = "1ADC.png";
		break;
	case 1:
		sequence = "1OLG.png";
		break;
	case 2:
		sequence = "1RLC.png";
		break;
	case 3:
		sequence = "2PTN.png";
		break;
	case 4:
		sequence = "1DGS.png";
		break;
	case 5:
		// WARNING!!
		sequence = "1KLN.png";
		break;
	case 6:
		sequence = "2EI2I.png";
		break;

	case 7:
		sequence = "2HMI.png";
		break;

	case 8:
		sequence = "1ASZ.png";
		break;

	case 9:
		// WARNING !!
		sequence = "1PPI.png";
		break;
	case 10:
		sequence = "5PEP.png";
		break;
	default:
		sequence = "";
	}
	return sequence;
}

std::string LevelHelper::getNameForLevel(int level) {
	std::string sequence = "";
	switch (level) {
	case 0:
		sequence = "Alcohol Dehydrogenase";
		break;
	case 1:
		sequence = "p53 Tumor Suppressor";
		break;
	case 2:
		sequence = "Rubisco (from Tabacco) (s. u.)";
		break;
	case 3:
		sequence = "Trypsin";
		break;
	case 4:
		sequence = "DNA Ligase";
		break;
	case 5:
		sequence = "DNA Polymerase I";
		break;
	case 6:
		sequence = "RNA Polymerase (chain E)";
		break;

	case 7:
		sequence = "Reverse Transkriptase (chain A)";
		break;

	case 8:
		sequence = "Aminoacyl-tRNA-Synthetase";
		break;

	case 9:
		// WARNING !!
		sequence = "Alpha-Amylase";
		break;
	case 10:
		sequence = "Pepsin";
		break;
	default:
		sequence = "";
	}
	return sequence;
}


int LevelHelper::getMaxSpeed(int level) {
	return 6;
}

int LevelHelper::getMinSpeed(int level) {
	return 4;
}

int LevelHelper::getMaxAA(int level) {
	return 9;
}

int LevelHelper::getMinAA(int level) {
	return 6;
}

//Every n calls of tick, one additional amino acid is added (if withing MIN and MAX)
int LevelHelper::getAAAddProb(int level) {
	return 230;
}

//Every n wall collisions, remove one amino acid.
int LevelHelper::getAARemProb(int level) {
	return 15;
}

