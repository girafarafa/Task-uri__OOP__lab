#ifndef TEST_H
#define TEST_H

void testAll();

// Teste Domain
void testGetters();
void testSetters();

// Teste Repo
void testRepoAdd();
void testRepoFind();
void testRepoIsin();
void testRepoSterge();
void testRepoModifica();
void testRepoGetActivitati();

// Teste Service
void testServiceAdd();
void testServiceSterge();
void testServiceModifica();
void testServiceCauta();
void testServiceFiltrareTip();
void testServiceFiltrareDescriere();
void testServiceSortTitlu();
void testServiceSortDescriere();
void testServiceSortDurata();

void testIterator();
void testRaport();
void testUndo();

void testFileRepo();
void testRepoMap();

void testExportCSVEroare();
void testGetIstoric();

#endif