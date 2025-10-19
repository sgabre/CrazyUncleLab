/**
 * @file Module.h
 * @brief Module Library Interface Declaration.
 * @ingroup public_interfaces
 */

#ifndef _MODULE_H_
#define _MODULE_H_

/**
* @brief Exécute une fonction publique du module.
* @ingroup public_interfaces
* 
* @param aValue Valeur à traiter.
*/
void Module_Public(int aValue);

/**
 * @brief Initialise le module.
 * @ingroup public_interfaces
 */
void Module_Setup(void);

#endif // _MODULE_H_