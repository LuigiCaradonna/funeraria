<?php

namespace App\Http\Controllers;

use App\Models\AccessoryType;
use Illuminate\Http\Request;
use Inertia\Inertia;

class AccessoryTypeController extends Controller
{
    /**
     * Shows the AccessoryTypes list
     * 
     * @param \Illuminate\Http\Request $request contains the eventual filters to customize the list
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the AccessoryTypes page
     */
    public function index(Request $request)
    {
        $search = $request->input('search') === null ? ""    : $request->input('search');
        $field  = $request->input('field')  === null ? "name": $request->input('field');
        $order  = $request->input('order')  === null ? 'ASC' : ($request->input('order') === 'true' ? 'ASC' : 'DESC');

        $accessory_types = AccessoryType::query();

        if ( $search != "" )
        {
            $accessory_types->where('name', 'like', "%{$search}%")->orderby($field, $order);
        }
        else 
        {
            $accessory_types->orderby($field, $order);
        }

        return Inertia::render('AccessoryTypes', [
            'accessory_types' => $accessory_types->paginate(20)->withQueryString(), // This works but VSCode doesn't know
            
            // Send to the client an object containing the value of the search querystring
            'filters' => $request->only(['search'])
        ]);
    }
    
    /**
     * Creates a new AccessoryType
     * 
     * @param \Illuminate\Http\Request $request contains the data to create the AccessoryType
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the AccessoryTypes page
     */
    public function store(Request $request)
    {
        if ( $request->input('id') != 0 )
            return redirect()->route('accessory_types')->with('error', 'Creazione tipo accessorio non riuscita.');

        // validate() returns the validated fields on success, they can be used to create the AccessoryType
        $validated = $request->validate([
            'name' => 'required'
        ]);

        // This is executed only if the validation succeedes
        if ( ! AccessoryType::create($validated) )
        {
            return redirect()->route('accessory_types')->with('error', 'Creazione tipo accessorio non riuscita.');
        }

        return redirect()->route('accessory_types')->with('success', 'Tipo accessorio creato con successo.');
    }

    /**
     * Updates an AccessoryType given its id
     * 
     * @param \Illuminate\Http\Request $request contains the data to update the AccessoryType
     * @param int $id id of the AccessoryType to update
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the AccessoryTypes page
     */
    public function edit(Request $request, $id)
    {
        if ( $id == 0 )
            return redirect()->route('accessory_types')->with('error', 'Tipo accessorio non trovato.');

        $request->validate([
            'name' => 'required'
        ]);

        $accessory_type = AccessoryType::find($id);

        if ( ! $accessory_type )
        {
            return redirect()->route('accessory_types')->with('error', 'Tipo accessorio non trovato.');
        }

        $accessory_type->name = $request->input('name');

        if ( ! $accessory_type->save() )
            return redirect()->route('accessory_types')->with('error', 'Modifica tipo accessorio non riuscita.');
            
        return redirect()->route('accessory_types')->with('success', 'Tipo accessorio modificato con successo.');
    }

    /**
     * Deletes an AccessoryType given its id
     * 
     * @param int $id id of the accessory to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the AccessoryTypes page
     */
    public function delete($id)
    {
        if ( $id <= 0 || $id == null )
            return redirect()->route('accessory_types')->with('error', 'Tipo accessorio non trovato.');

        $accessory_type = AccessoryType::find($id);

        if ( ! $accessory_type )
        {
            return redirect()->route('accessory_types')->with('error', 'Tipo accessorio non trovato.');
        }

        if ( ! $accessory_type->delete() )
            return redirect()->route('accessory_types')->with('error', 'Eliminazione tipo accessorio non riuscita.');
            
        return redirect()->route('accessory_types')->with('success', 'Tipo accessorio eliminato con successo.');
    }
    
    /**
     * Deletes a set of AccessoryTypes whose ids are conteined into the "ids" parameter coming from the request
     * 
     * @param \Illuminate\Http\Request $request containing the ids of the AccessoryTypes to delete
     * 
     * @return \Illuminate\Http\RedirectResponse Call to the AccessoryTypes page
     */
    public function multiDelete(Request $request)
    {
        if ( AccessoryType::destroy(collect($request->input('ids'))) )
            return redirect()->route('accessory_types')->with('success', 'Tipi accessori eliminati con successo.');

        return redirect()->route('accessory_types')->with('error', 'Eliminazione tipi accessori non riuscita.');
    }
}
