# DTD Validator for XML 

Jocelyn Vernay, 2017.

This is a project I realized during my M2 Data & Knowledge, in the Web Data Models course. The goal was to reproduce the process of DTD validation of a XML document.

In this project, we process the streaming format of XML and a simplified version of DTD.

XML : 

    0 a
    0 b
    1 b
    0 c
    1 c
    0 d
    1 d
    1 a



DTD : 

	a (bc)*d
	b _
	c _
	d _